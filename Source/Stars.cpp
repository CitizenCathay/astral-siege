/******************************************************************************/
/**
 * @file        Stars.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (primary)
 * @brief       Handles star animations and rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <crtdbg.h>
#include <cmath>
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/Stars.hpp"

float driftX = 0.0f;				// Horizontal drift offset
float driftY = 0.0f;				// Vertical drift offset
float const driftSpeedX = 7.5f;		// Drift speed along X-axis (negative for leftward motion)
float const driftSpeedY = -10.0f;	// Drift speed along Y-axis (negative for upward motion)

// Star field management
std::vector<Star> stars;
const int NUM_STARS = 450;  // Increased number of stars for better effect

void InitializeStars() {
	stars.clear();
	for (int i = 0; i < NUM_STARS; ++i) {
		float x = static_cast<float>(rand() % 1600 - 800);  // Range: -800 to 800
		float y = static_cast<float>(rand() % 900 - 450);   // Range: -450 to 450
		stars.push_back(Star(x, y));
	}
}

void UpdateStars(float deltaTime) {
	for (auto& star : stars) {
		// Update star state
		star.update(deltaTime);
	}

	// Update the drift offset
	driftX += driftSpeedX * deltaTime;
	driftY += driftSpeedY * deltaTime;

	// Wrap around to prevent excessive values (optional, for looping effect)
	if (driftX < -1600.0f) driftX += 1600.0f; // Screen width
	if (driftY < -900.0f) driftY += 900.0f;   // Screen height
}

void RenderStars(AEGfxVertexList* regularStarMesh, AEGfxVertexList* supernovaStarMesh) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	for (auto& star : stars) {
		// Select the appropriate mesh
		AEGfxVertexList* mesh = star.isSupernova ? supernovaStarMesh : regularStarMesh;

		// Apply different scaling for supernova stars
		float scaleFactor = star.isSupernova ? 6.0f : 2.0f; // Exaggerate supernova size

		// Set the transform with the appropriate scale factor
		AEGfxSetTransform(GetStarTransform(
			star.getRenderX(),
			star.getRenderY(),
			star.size * scaleFactor,
			star.size * scaleFactor
		).m);

		// Set transparency based on brightness
		AEGfxSetTransparency(star.brightness);

		// Draw the star
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}
}