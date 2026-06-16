/******************************************************************************/
/**
 * @file        TutorialsMenu.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (Primary)
 * @brief       Manages the tutorial menu screen, including displaying tutorial
 *              instructions and handling user interactions to navigate through
 *              gameplay mechanics and controls.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/Game.hpp"
#include "../Header/UIManager.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/AudioPlayer.hpp"

// Texture variables
static AEGfxTexture* tutorialBackgroundTex = nullptr;
static AEGfxVertexList* backgroundMesh = nullptr;

void Tutorial_Load() {
    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();
    // Load your tutorial image
    tutorialBackgroundTex = AEGfxTextureLoad("Assets/tut.png");

    // create and initialize the UI manager
    uiManager = new UIManager();
    uiManager->InitializeFonts();
}

void Tutorial_Initialize() {
    // Create star meshes first
    starMesh = createCircleMesh(0xFFFFFFFF);
    supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

    InitializeStars(); // Initialize the star field

    // Create a fullscreen quad mesh
    AEGfxMeshStart();

    // First triangle (Bottom left, Bottom right, Top left)
    AEGfxTriAdd(
        -1.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,  // Bottom left
        1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,   // Bottom right
        -1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f    // Top left
    );

    // Second triangle (Bottom right, Top right, Top left)
    AEGfxTriAdd(
        1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,   // Bottom right
        1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,    // Top right
        -1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f    // Top left
    );

    backgroundMesh = AEGfxMeshEnd();

    ShowConfirmation = false;
}

void Tutorial_Update() {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    UpdateStars(dt); // Update star positions

    BackButton_Update();
    PopUp_Update();
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        ShowConfirmation = !ShowConfirmation;
    }
}

void Tutorial_Draw() {
    // Render stars first
    RenderStars(starMesh, supernovaStarMesh);

    // Always reset render state at the start
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // Window-sized scaling
    int windowWidth = AEGfxGetWindowWidth();
    int windowHeight = AEGfxGetWindowHeight();

    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, windowWidth / 2.0f, windowHeight / 2.0f);

    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, 0.0f, 0.0f);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &translate, &scale);
    AEGfxSetTransform(transform.m);

    // Apply texture
    AEGfxTextureSet(tutorialBackgroundTex ? tutorialBackgroundTex : NULL, 0, 0);
    AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

    // Draw the title "CREDITS" at the top
    uiManager->DrawTitle("TUTORIAL");

    BackButton_Draw();
}

void Tutorial_Free() {
    AEGfxMeshFree(starMesh);
    starMesh = nullptr;

    AEGfxMeshFree(supernovaStarMesh);
    supernovaStarMesh = nullptr;

    MeshManager::GetInstance().UnloadTexture();
    MeshManager::GetInstance().UnloadMesh();

    // Free the mesh
    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }

    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;
}

void Tutorial_Unload() {
    // Unload the texture
    if (tutorialBackgroundTex) {
        AEGfxTextureUnload(tutorialBackgroundTex);
        tutorialBackgroundTex = nullptr;
    }
}