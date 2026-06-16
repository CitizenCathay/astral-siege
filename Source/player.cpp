/******************************************************************************/
/**
 * @file        Player.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary) - 80%
 * @author      Guok Yi Yong (secondary) - Render the player with a camera
				system, and add a dynamic screenshake effect to the health bar (20%)
 * @brief       This file defines Earth behaviors and Screen Shake
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
				All rights reserved.
 */
 /******************************************************************************/

#include <math.h>
#include <vector>
#include "../Header/Player.hpp"
#include "../Header/Camera.hpp"
#include "../Header/CurrencyManager.hpp"
#include "../Header/MeshManager.hpp"

extern AEGfxVertexList* pMeshRect;

AEMtx33 TransformHealthRect = { 0 };
AEMtx33 TransformCircle[3] = { 0 };

float HEALTHBAR_centerX = 0.0f;
float HEALTHBAR_ypos = 400.0f;
static const float Screen_Width = 1600.0f;

//--------------------------------------------------------------------------
// Set Up Circles
//--------------------------------------------------------------------------
void EarthInit() {
	// Earth position and scale transformation
	AEMtx33 Scale, Trans;

	// Apply scale first
	AEMtx33Scale(&Scale, EARTH_CIRCLE_SCALE, EARTH_CIRCLE_SCALE);

	// Then position
	AEMtx33Trans(&Trans, EarthPosition.x, EarthPosition.y);

	// Combine matrices: scale first, then translate
	AEMtx33 tempMat;
	AEMtx33Concat(&tempMat, &Trans, &Scale);
	TransformCircle[1] = tempMat;
}

//--------------------------------------------------------------------------
// Set Up Rectangles
//--------------------------------------------------------------------------
void EarthHealthBar(float Health) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Background Health Bar
	AEMtx33 ScaleBackground = { 0 };
	AEMtx33Scale(&ScaleBackground, 40.f, HEALTH_BAR_WIDTH);

	AEMtx33 TransBackground = { 0 };
	AEMtx33Trans(&TransBackground, HEALTHBAR_centerX, HEALTHBAR_ypos);

	AEMtx33 RotateBackground = { 0 };
	AEMtx33Rot(&RotateBackground, -PI / 2);

	AEMtx33 backgroundTransform;
	AEMtx33Concat(&backgroundTransform, &TransBackground, &RotateBackground);
	AEMtx33Concat(&backgroundTransform, &backgroundTransform, &ScaleBackground);

	ApplyCameraShake(backgroundTransform);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(0.5f, 0.0f, 0.0f, 1.0f);
	AEGfxSetTransform(backgroundTransform.m);
	AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::HEALTHBAR), AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Update Health Bar Transform
	// Calculate scaled width
	float scaledWidth = HEALTH_BAR_WIDTH * (Health / MAX_HEALTH);

	// Shift X to anchor from left
	float offsetX = -(HEALTH_BAR_WIDTH - scaledWidth) / 2.0f;

	AEMtx33 ScaleHealth = { 0 };
	AEMtx33Scale(&ScaleHealth, scaledWidth, 40.f);

	AEMtx33 TransHealth = { 0 };
	AEMtx33Trans(&TransHealth, HEALTHBAR_centerX + offsetX, HEALTHBAR_ypos);

	AEMtx33Concat(&TransformHealthRect, &TransHealth, &ScaleHealth);

	// Apply camera shake to the health bar
	ApplyCameraShake(TransformHealthRect);

	// Set render mode and draw the health bar
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(TransformHealthRect.m);
	AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::HEALTHBAR), AE_GFX_MDM_TRIANGLES);

	// Reset score if health reaches 0
	if (Health <= 0) {
		CurrencyManager::GetInstance().Reset();
	}
}

void EarthRender() {
	// Set up for textured rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Get the Earth texture from MeshManager instead of SpriteManager
	AEGfxTexture* earthTexture = MeshManager::GetInstance().GetTexture(TextureType::EARTH);
	AEGfxTextureSet(earthTexture, 0, 0);

	// Apply camera zoom to the Earth's transform
	Camera* camera = CameraManager::GetInstance().GetCurrentCamera();
	AEMtx33 finalTransform = TransformCircle[1];

	// Apply camera zoom
	AEMtx33 zoomMatrix;
	AEMtx33Scale(&zoomMatrix, camera->cameraZoom, camera->cameraZoom);
	AEMtx33Concat(&finalTransform, &zoomMatrix, &finalTransform);

	// Apply camera shake to the Earth
	ApplyCameraShake(finalTransform);

	// Set the transform and draw the Earth using the QUAD mesh
	AEGfxSetTransform(finalTransform.m);
	AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

	// Reset render mode for other elements
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

// Function to reset Earth’s health to full
void ResetEarthHealth() {
	EarthHealth = MAX_HEALTH;  // Reset health to max
}



