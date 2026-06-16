/******************************************************************************/
/**
 * @file        MainMenuObjects.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       Handles logic related to the animated ship object in the main menu screen,
 *              including movement and rendering behavior.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/MainMenuObjects.hpp"
#include "AEEngine.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// Define ship variables
float MainMenuObjects::shipX;
float MainMenuObjects::shipY;
float MainMenuObjects::shipSpeed;
float MainMenuObjects::shipDirectionX;
float MainMenuObjects::shipDirectionY;
float MainMenuObjects::shipRotation;
float MainMenuObjects::shipRotationOffset = 0.0f;
AEGfxVertexList* shipMesh = nullptr;
AEGfxTexture* shipTexture = nullptr;

void MainMenuObjects::Initialize() {
    srand(static_cast<unsigned int>(time(0)));

    AEGfxMeshStart();
    // Create a quad mesh with texture coordinates
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left
        -0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f,  // top-left
         0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f   // top-right
    );
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left
         0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f,  // top-right
         0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f   // bottom-right
    );
    shipMesh = AEGfxMeshEnd();

	shipTexture = AEGfxTextureLoad("Assets/ship.png");

    shipRotationOffset = 90.0f;

    ResetShip();
}

void MainMenuObjects::ResetShip() {
    // ues game coordinates 
    float maxX = 800.0f;
    float maxY = 450.0f;

	// choose a random side to spawn the ship
    int side = rand() % 4;
    switch (side) {
    case 0: // Left side
        shipX = -maxX - 50.0f;
        shipY = (rand() % (int)(maxY * 2)) - maxY;
        break;
    case 1: // Right side
        shipX = maxX + 50.0f;
        shipY = (rand() % (int)(maxY * 2)) - maxY;
        break;
    case 2: // Top side
        shipX = (rand() % (int)(maxX * 2)) - maxX;
        shipY = maxY + 50.0f;
        break;
    case 3: // Bottom side
        shipX = (rand() % (int)(maxX * 2)) - maxX;
        shipY = -maxY - 50.0f;
        break;
    }

    // Random speed between 100 and 200
    shipSpeed = (rand() % 100) + 100.0f;

    // Random direction vector
    shipDirectionX = static_cast<float>(rand() % 200) - 100;
    shipDirectionY = static_cast<float>(rand() % 200) - 100;

    // Normalize direction
    float length = sqrt(shipDirectionX * shipDirectionX + shipDirectionY * shipDirectionY);
    shipDirectionX /= length;
    shipDirectionY /= length;
}

void MainMenuObjects::Update(float dt) {
    // Move ship in the current direction
    shipX += shipDirectionX * shipSpeed * dt;
    shipY += shipDirectionY * shipSpeed * dt;

    // Check if ship is off-screen (using 1600x900 dimensions)
	float maxX = 850.0f; // position slightly off-screen
    float maxY = 500.0f;

    if (shipX < -maxX || shipX > maxX || shipY < -maxY || shipY > maxY) {
        // resets ship w a new random path to follow
        ResetShip();
    }

    // Update ship rotation to face movement direction
    shipRotation = atan2(-shipDirectionY, -shipDirectionX) * (180.0f / 3.14159f);
}

void MainMenuObjects::Draw() {
    AEGfxTextureSet(shipTexture, 0, 0);

    // Set the render properties for visibility
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // transformation matrices
    AEMtx33 transform = { 0 };
    AEMtx33 scale = { 0 };
    AEMtx33 rotate = { 0 };
    AEMtx33 translate = { 0 };
    AEMtx33 temp = { 0 };

    // Scale the ship
    AEMtx33Scale(&scale, 60.0f, 60.0f);

    // Use the calculated rotation directly (already in degrees)
    // Convert back to radians for the matrix function
    AEMtx33Rot(&rotate, shipRotation * (3.14159f / 180.0f));

    float rotationRadians = (shipRotation + 90.0f) * (3.14159f / 180.0f);

    AEMtx33Rot(&rotate, rotationRadians);

    // Position the ship
    AEMtx33Trans(&translate, shipX, shipY);

    // concat transformation - order matters!
    AEMtx33Concat(&temp, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &temp);

    // Set the transform
    AEGfxSetTransform(transform.m);

    // Draw the mesh
    AEGfxMeshDraw(shipMesh, AE_GFX_MDM_TRIANGLES);
}

void MainMenuObjects::Cleanup() {
    // Unload the ship texture
    if (shipTexture) {
        AEGfxTextureUnload(shipTexture);
        shipTexture = nullptr;
    }
}