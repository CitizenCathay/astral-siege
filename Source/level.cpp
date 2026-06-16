/******************************************************************************/
/**
 * @file        Level.cpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       Manages game levels and their properties.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include <vector>
#include "../Header/MeshManager.hpp"
#include "../Header/GameObject.hpp"
#include "../Header/Stars.hpp"
#include "../Header/Vector.hpp"
#include "../Header/GravitySystem.hpp"
#include "../Header/Particles.hpp"
#include "../Header/Missle.hpp"
#include "../Header/WeaponManager.hpp"
#include "../Header/Weapon.hpp"
#include "../Header/Camera.hpp"
#include "../Header/Level.hpp"
#include "../Header/Enemy.hpp"
#include "../Header/EnemyBullet.hpp"
#include "../Header/Player.hpp"
#include "../Header/DifficultyManager.hpp"
#include "../Header/CurrencyManager.hpp"

LevelData* levels = NULL;  
int levelCount = 0;
extern int currentLevel;
extern int levelCounts;


void Cameralevel()
{
    levels = (LevelData*)malloc(MAX_LEVELS * sizeof(LevelData));
    if (!levels)
    {
        exit(1);
    }
    levels[0].cameraLevel = 1.7f;
    levels[1].cameraLevel = 1.1f;
    levels[2].cameraLevel = 0.7f;
}

// This function gets called when player buys telescope upgrade
void ShopZoomOut(Camera* camera) {
    if (levelCount < 2) {  // Avoid going out of bounds
        ++levelCount;

        // Apply the new camera zoom
        if (camera != NULL) {
            camera->cameraZoom = levels[levelCount].cameraLevel;
        }
    }
}


float GetCameralevel()
{
    return static_cast<float>(levelCount);
}

// Outerplanet constructor
Outerplanet::Outerplanet(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, int textureType)
    : GameObject(pos, scale, rotation, mesh, isActive, textureType)
{
}


void Outerplanet::Draw()
{
    // Ensure the texture is valid before setting it
    AEGfxTexture* currentTexture = nullptr;

    // Check if the texture type is valid and assign the texture accordingly
    switch (this->textureType) {
    case TextureType::VENUS:
        currentTexture = MeshManager::GetInstance().GetTexture(TextureType::VENUS);
        break;
    case TextureType::MOON:
        currentTexture = MeshManager::GetInstance().GetTexture(TextureType::MOON);
        break;
    case TextureType::SUN:
        currentTexture = MeshManager::GetInstance().GetTexture(TextureType::SUN);
        break;
    case TextureType::URANUS:
        currentTexture = MeshManager::GetInstance().GetTexture(TextureType::URANUS);
        break;
    case TextureType::SATURN:
		currentTexture = MeshManager::GetInstance().GetTexture(TextureType::SATURN);
		break;
    default:
        currentTexture = nullptr;  // Default case if texture type is invalid
        break;
    }

    // If the texture is valid, apply it to the mesh
    if (currentTexture) {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // Use texture mode for rendering
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);      // Set blending mode
        AEGfxSetTransparency(1.0f);               // Set transparency level (full opacity)

        // Apply the selected texture to the mesh
        AEGfxTextureSet(currentTexture, 0, 0);
    }

    // Set the transformation matrix (position, scale, rotation)
    AEGfxSetTransform(GetTransform().m);

    // Check if the mesh is valid before trying to draw
    if (pMesh) {
        // Draw the mesh with the selected texture
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }
}




void ResetGameState(Camera *camera) {
    // Store current audio volumes before reset
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    //Back button
    ShowConfirmation = false;
    ShowQuitConfirmation = false;
    PauseShowConfirmation = false;

    // Game Win
    enemiesDestroyed = 0;
    levelCount = 0;
    levelCounts = 0;
    currentLevel = 0;
    camera->cameraZoom = 1.7f;
    camera->position = { 0.0f, 0.0f }; // Reset camera position to origin
    cameraPosX = 0.0f;  // Start towards the right side of a 1600-wide map
    cameraPosY = 0.0f;    // Center vertically

    // Reset the transformation matrix
    //AEMtx33Identity(&camera->transformMatrix);
    Missle::Free();
    outerPlanetManager.Clear();  // Ensure old assets are removed
    outerPlanetManager.Init(currentLevel);  // Now reinitialize at level 0
    CurrencyManager::GetInstance().Reset();

    MeshManager::GetInstance();

    // Reinitialize explosions
    void ResetExplosions(std::vector<Explosion>&explosions);

    // Refresh weapon state
    WeaponManager::GetInstance().ResetWeapons();  // Reset weapon state, make sure this method is defined

    // Reset bullets
    ResetBullets();  // this function resets bullets, define it if not yet implemented

    // Reset enemies
    ResetEnemies();  // this function resets all enemy positions and states

    // Adjust the camera position after resetting everything else
    AdjustCameraPosition();  // Make sure this function adjusts the camera properly

    // Reinitialize stars
    InitializeStars();  // this function reinitializes the stars in the game

    // Reset Earth health
    ResetEarthHealth();  // this function resets Earth�s health properly

    if (hud) {
        hud->Reset();
    }

    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume);
}

void ResetGameLevel(Camera* camera) {
    // Store current audio volumes before reset
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    enemiesDestroyed = 0;
    // Reset camera
    camera->position = { 0.0f, 0.0f }; // Reset camera position to origin
    cameraPosX = 0.0f;  // Start towards the right side of a 1600-wide map
    cameraPosY = 0.0f;    // Center vertically

    Missle::Free();

    MeshManager::GetInstance();

    // Reinitialize explosions
    void ResetExplosions(std::vector<Explosion>&explosions);

    // Reset bullets
    ResetBullets();  // this function resets bullets, define it if not yet implemented

    // Reset enemies
    ResetEnemies();  // this function resets all enemy positions and states

    // Adjust the camera position after resetting everything else
    AdjustCameraPosition();  // Make sure this function adjusts the camera properly

    // Reinitialize stars
    InitializeStars();  // this function reinitializes the stars in the game

    // Reset Earth health
    ResetEarthHealth();  // this function resets Earth�s health properly

    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume);
}