/******************************************************************************/
/**
 * @file        Camera.cpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       Implementation of the Camera class, which handles camera movement,
 *              zoom, and transformations in the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <crtdbg.h>
#include <cmath>
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/Camera.hpp"

// Camera zoom variables
const float cameraMoveSpeed = 10.0f; // Speed at which the camera moves
const float zoomLevels[] = { 0.5f,0.8f, 1.2f, 1.6f, 2.0f };  // Predefined zoom levels
const int numZoomLevels = sizeof(zoomLevels) / sizeof(zoomLevels[0]);
int zoomIndex = 4;
float cameraZoom = 1.0f;  // Default zoom level

//Camera zoom variables for gameplay
const float ZOOM_STEP = 0.5f;
const float MIN_ZOOM = 0.5f;
const float MAX_ZOOM = 2.0f;

const float SHOP_ZOOM_STEP = 0.6f;

// Camera position variables
float cameraPosX = 0.0f;
float cameraPosY = 0.0f;
float cameraScale = zoomLevels[zoomIndex];

const float mapWidth = 1600.0f;
const float mapHeight = 900.0f;

// Camera shake variables
float shakeDuration = 0.0f;
float shakeIntensity = 0.0f;
float shakeElapsed = 0.0f;

// Initialize camera position and zoom
void InitCamera(Camera* camera) {
    cameraScale = zoomLevels[zoomIndex]; // Set initial zoom
    cameraPosX = ::mapWidth / 2.0f; // Centered near the right side, :: operator uses global versions
    cameraPosY = ::mapHeight / 2.0f;

    ApplyCameraTransform(camera);
}

// Apply camera transformations
void ApplyCameraTransform(Camera* camera) {
    AEMtx33 scaleMatrix, translationMatrix;

    // Scaling matrix (zoom effect)
    AEMtx33Scale(&scaleMatrix, cameraScale, cameraScale);

    // Translate to keep the camera centered
    AEMtx33Trans(&translationMatrix, -cameraPosX * cameraScale, -cameraPosY * cameraScale);

    // Combine transformations: Scale first, then translate
    AEMtx33Concat(&camera->transformMatrix, &scaleMatrix, &translationMatrix);

    // Apply transformation to graphics engine
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransform(camera->transformMatrix.m);
}
void ZoomIn(Camera* camera) {
    camera->cameraZoom += ZOOM_STEP;
    if (camera->cameraZoom > MAX_ZOOM) camera->cameraZoom = MAX_ZOOM;}

void ZoomOut(Camera* camera) {
    camera->cameraZoom -= ZOOM_STEP;
    if (camera->cameraZoom < MIN_ZOOM) camera->cameraZoom = MIN_ZOOM;
}

void AdjustCameraToBounds() {
    float halfScreenWidth = (mapWidth / cameraScale) / 2;
    float halfScreenHeight = (mapHeight / cameraScale) / 2;

    cameraPosX = fmaxf(halfScreenWidth, fminf(cameraPosX, mapWidth - halfScreenWidth));
    cameraPosY = fmaxf(halfScreenHeight, fminf(cameraPosY, mapHeight - halfScreenHeight));
}

// Function to adjust camera position based on zoom factor
void AdjustCameraPosition() {
    // Define camera boundaries (adjust as needed)
    const float minX = -1600.0f;
    const float maxX = 1600.0f;
    const float minY = -900.0f;
    const float maxY = 900.0f;

    // Clamp camera position within bounds
    cameraPosX = fmaxf(minX, fminf(cameraPosX, maxX));
    cameraPosY = fmaxf(minY, fminf(cameraPosY, maxY));

    AEGfxSetCamPosition(cameraPosX, cameraPosY);
}

void StartCameraShake(float duration, float intensity) {
    shakeDuration = duration;
    shakeIntensity = intensity;
    shakeElapsed = 0.0f;
}

void StopCameraShake() {
    shakeDuration = 0.0f;  // Stop shake instantly
    shakeIntensity = 0.0f; // Reset intensity
}


void ApplyCameraShake(AEMtx33& cameraTransform) {
    if (shakeElapsed < shakeDuration) {
        // Generate random offsets in range [-shakeIntensity, +shakeIntensity]
        float offsetX = ((rand() % 100) / 100.0f) * shakeIntensity * 0.5f - shakeIntensity * 0.2f;
        float offsetY = ((rand() % 100) / 100.0f) * shakeIntensity * 0.5f - shakeIntensity * 0.2f;

        // Apply offset to camera transform
        AEMtx33 TransShake;
        AEMtx33Trans(&TransShake, offsetX, offsetY);
        AEMtx33Concat(&cameraTransform, &cameraTransform, &TransShake);

        // Update elapsed time
        shakeElapsed += 0.5f / 60.0f; // Assuming 60 FPS
    }
}

// Singleton
CameraManager& CameraManager::GetInstance() {
    static CameraManager camMng;
    return camMng;
}

CameraManager::CameraManager() {
    AddCamera(); // add in main camera
    SetCurrentCamera(CAMERA_MODE::MAIN_CAM);
}

void CameraManager::AddCamera() {
    cameras.push_back(Camera());
}

void CameraManager::SetCurrentCamera(CAMERA_MODE cm) {
    selectedCamera = &cameras[static_cast<int>(cm)];
}

Camera* CameraManager::GetCurrentCamera() {
    return selectedCamera;
}