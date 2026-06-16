/******************************************************************************/
/**
 * @file        GravitySystem.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary) - 90%
 * @author      Guok Yi Yong (Secondary) - Render the gravitypoint with a camera system (10%)
 * @brief       This file manages gravitational forces affecting game objects.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
				All rights reserved.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include <vector>
#include "../Header/GravitySystem.hpp"
#include "../Header/GameObject.hpp"
#include "../Header/Camera.hpp"


void RenderGravityPoints(const std::vector<GravityPoint>& gravityPoints, AEGfxVertexList* gravityMesh, Camera* camera) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransparency(0.7f); // Slight transparency

	for (const GravityPoint& point : gravityPoints) {
		// Use camera zoom
		float zoom = camera->cameraZoom;

		// Scale planet size based on zoom
		float planetScale = point.size * zoom;

		// Adjust position based on zoom
		float adjustedX = point.x * zoom;
		float adjustedY = point.y * zoom;

		// Set the transform for the gravity point with zoom factor
		AEMtx33 transform = GetTransform(adjustedX, adjustedY, planetScale, planetScale);
		AEGfxSetTransform(transform.m);

		// Render the gravity point
		AEGfxMeshDraw(gravityMesh, AE_GFX_MDM_TRIANGLES);
	}
}


