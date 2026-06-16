/******************************************************************************/
/**
 * @file        Camera.hpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       This file defines the Camera class and CameraManager for managing camera
 *              positioning, zoom, and transformations
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <vector>
#include "../Header/GameObject.hpp"

extern float cameraPosX;
extern float cameraPosY;
extern float cameraScale;
extern float shakeIntensity;
extern float shakeDuration;
extern float shakeElapsed;
extern float shakeTimeElapsed;
extern float transitionTime;
extern float cameraZoom;
extern const float ZOOM_STEP;
extern const float MIN_ZOOM;
extern const float MAX_ZOOM;

extern const float SHOP_ZOOM_STEP;

enum class CAMERA_MODE {
    MAIN_CAM,
    TOTAL_CAM
};

extern int currentZoomStep;
extern const int MAX_ZOOM_STEPS;

class Camera {
public:
    AEVec2 position;  // Camera position
    AEMtx33 transformMatrix; // Transformation matrix
    float cameraZoom = 1.7f; 


    Camera() {
        position = { 0.0f, 0.0f };
        AEMtx33Identity(&transformMatrix);
    }
};



class CameraManager {
public:
    static CameraManager& GetInstance();
    void AddCamera();
    Camera* GetCurrentCamera();
    void SetCurrentCamera(CAMERA_MODE cm);
    CameraManager(CameraManager const&) = delete; // dont allow copy of Weapon manager
    void operator=(CameraManager const&) = delete;  // dont allow assignment of Weapon manager
private:
    Camera* selectedCamera;
    std::vector<Camera> cameras;
    CameraManager();// dont allow constructing of Weapon manager
};

// Function declarations for camera operations

void InitCamera(Camera* camera);
void ZoomIn(Camera* camera);
void ZoomOut(Camera* camera);
void AdjustCameraToBounds();
void AdjustCameraPosition();
void ApplyCameraTransform(Camera* camera);
void StartCameraShake(float duration, float intensity);
void StopCameraShake();
void ApplyCameraShake(AEMtx33& cameraTransform);