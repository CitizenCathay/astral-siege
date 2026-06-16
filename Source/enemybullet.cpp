/******************************************************************************/
/**
 * @file        EnemyBullet.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary) - 90%
 * @author      Guok Yi Yong (secondary) - added cameera features (10%)
 * @brief       Manages the creation, movement, rendering, and collision handling
 *              of enemy bullets in the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/


#include "..\Header\EnemyBullet.hpp"
#include "..\Header\Camera.hpp"
#include "..\Header\AudioPlayer.hpp"
#include "..\Header\MeshManager.hpp"

static BulletEnemy bullets[MAX_BULLETS];  // Array to store bullets
// Get the current camera.
Camera* camera = CameraManager::GetInstance().GetCurrentCamera();

void BulletCreate(float StartX, float StartY, float VelocityX, float VelocityY) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].In_Game) {  // if inactive, bullet restart bullets
            bullets[i].Position.x = StartX;
            bullets[i].Position.y = StartY;
            bullets[i].Velocity.x = VelocityX;
            bullets[i].Velocity.y = VelocityY;
            bullets[i].In_Game = true;
            break;  // Only spawn one bullet at a time
        }
    }
}

void BulletMovement(float dt) {
    // Retrieve window dimensions
    float windowWidth = static_cast<float>(AEGfxGetWindowWidth());
    float windowHeight = static_cast<float>(AEGfxGetWindowHeight());

    // Compute half-dimensions in world space (accounting for zoom)
    float halfWidth = windowWidth / (2.0f * camera->cameraZoom);
    float halfHeight = windowHeight / (2.0f * camera->cameraZoom);

    // Determine visible boundaries in world coordinates
    float leftBoundary = camera->position.x - halfWidth;
    float rightBoundary = camera->position.x + halfWidth;
    float bottomBoundary = camera->position.y - halfHeight;
    float topBoundary = camera->position.y + halfHeight;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].In_Game) {
            bullets[i].Position.x += bullets[i].Velocity.x * dt;
            bullets[i].Position.y += bullets[i].Velocity.y * dt;

            // Deactivate bullet if it goes off-screen relative to the current camera view.
            if (bullets[i].Position.x < leftBoundary || bullets[i].Position.x > rightBoundary ||
                bullets[i].Position.y < bottomBoundary || bullets[i].Position.y > topBoundary) {
                bullets[i].In_Game = false;
            }
        }
    }
}

void BulletRender() {
    Camera* currentCamera = CameraManager::GetInstance().GetCurrentCamera();
    AEMtx33 camScale;
    AEMtx33Scale(&camScale, currentCamera->cameraZoom, currentCamera->cameraZoom);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].In_Game) {
            AEGfxSetTransparency(1.0f);

            AEMtx33 bulletScale;
            AEMtx33Scale(&bulletScale, 5.0f, 5.0f); 

            AEMtx33 bulletTrans;
            AEMtx33Trans(&bulletTrans, bullets[i].Position.x, bullets[i].Position.y); 

            AEMtx33 transform, temp;
            AEMtx33Concat(&temp, &bulletTrans, &bulletScale);  
            AEMtx33Concat(&transform, &camScale, &temp);       

            AEGfxSetTransform(transform.m);
            AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::ENEMYBULLET), AE_GFX_MDM_TRIANGLES);
        }
    }
}

void CheckBulletCollisions(float& EarthHealth, AEVec2& EarthPosition, float EarthRadius) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].In_Game) {
            // Use AETestCircleToCircle for collision detection
            if (AETestCircleToCircle(&bullets[i].Position, 5.0f, &EarthPosition, EarthRadius)) {
                bullets[i].In_Game = false;     // Deactivate bullet upon collision
                EarthHealth -= 5.0f;            // Decrease Earth's health
                StartCameraShake(0.1f, 0.1f);   // Shake for 0.1 seconds with intensity 0.1
				AudioPlayer::GetInstance().PlayTakeDamage();  // Play damage sound effect

                if (EarthHealth < 0.0f) {
                    EarthHealth = 0.0f;  // Prevent negative health
                }
            }
        }
    }
}

void ResetBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].In_Game = false;             // Mark bullets as inactive
        bullets[i].Position = { 0.0f, 0.0f };   // Reset position
    }
}

BulletEnemy* GetEnemyBullet() {
    return bullets;
}