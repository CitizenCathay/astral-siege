/******************************************************************************/
/**
 * @file        Stars.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This header file defines the Star structure used for rendering twinkling
 *              and animated stars in the game. It manages brightness, size, and supernova
 *              effects, updating properties over time. Functions include initialization,
 *              updating, and rendering of stars.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <cmath>
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/GameObject.hpp"
#include "../Header/MeshManager.hpp"

extern float driftX;			// Horizontal drift offset
extern float driftY;			// Vertical drift offset
extern const float driftSpeedX; // Drift speed along X-axis (negative for leftward motion)
extern const float driftSpeedY; // Drift speed along Y-axis (negative for upward motion)

/*!************************************************************************
\brief
	Represents a star with animated twinkle, wobble, and supernova effects.

\details
	Holds position, brightness, size, and state data. Uses trigonometric functions and
	randomization for smooth animations and screen wrapping.
**************************************************************************/
struct Star {
	float x, y;                 // Base position
	float offsetX, offsetY;     // Current offset from base position
	float brightness;           // Current brightness (0.0 to 1.0)
	float twinkleSpeed;         // Speed of brightness change
	float twinkleTime;          // Current time in twinkle cycle
	float wobbleRadius;         // How far the star can move from its base position
	float wobbleTime;			// Time tracker for position wobble
	float size;					// Current size of the star
	float sizeSpeed;			// Speed of size fluctuation
	float sizeTime;				// Time tracker for size animation
	bool isSupernova;			// Whether this star is a supernova
	float supernovaTime;		// Time the star has been in supernova state
	const float supernovaDuration = 5.0f; // Duration of supernova state in seconds

	/*!************************************************************************
	\brief
		Constructs a Star with an initial position.

	\param posX
		The initial x-coordinate.

	\param posY
		The initial y-coordinate.

	\details
		Initializes star properties with random values for twinkle, wobble, and size animations while setting default supernova state.
	**************************************************************************/
	Star(float posX, float posY) {
		x = posX;
		y = posY;
		offsetX = offsetY = 0.0f;
		brightness = static_cast<float>(rand()) / RAND_MAX;						// Random initial brightness
		twinkleSpeed = 1.0f + (static_cast<float>(rand()) / RAND_MAX) * 2.0f;	// Random speed
		twinkleTime = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;			// Random start phase
		wobbleRadius = 0.5f;													// Maximum 0.5 units of movement
		wobbleTime = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
		size = 1.0f;															// Default size
		sizeSpeed = 0.5f + static_cast<float>(rand()) / RAND_MAX;				// Random speed
		sizeTime = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;			// Random start phase
		isSupernova = false;													// Default to regular star
		supernovaTime = 0.0f;
	}

	/*!************************************************************************
	 \brief
	  Updates star animations and state over time.

	 \param deltaTime
	  Time elapsed since the last update (in seconds).

	 \details
	  Adjusts twinkle phase, brightness, size, and wobble offsets. Handles supernova state transitions and wraps the star when off-screen.
	**************************************************************************/
	void update(float deltaTime) {
		// Update twinkle
		twinkleTime += twinkleSpeed * deltaTime;
		if (twinkleTime > 2 * M_PI) {
			twinkleTime -= 2 * M_PI;
		}

		// Brightness varies between 0.1 and 1.0
		// Add occasional bursts of brightness
		if (rand() % 1000 < 5) { // ~0.5% chance per update
			brightness = 1.0f; // Full brightness for a "pop" effect
		}
		else {
			brightness = 0.3f + 0.9f * (0.5f + 0.5f * sin(twinkleTime)); // Regular twinkle
		}

		// Random chance for a "pop" effect
		if (rand() % 1000 < 3) { // Adjust probability as needed
			size = 2.0f; // Temporary size increase
			brightness = 1.0f; // Full brightness
		}
		else if (size > 1.5f) {
			size -= 0.1f; // Gradually return to normal size
		}

		// Update position wobble
		wobbleTime += 0.2f * deltaTime;  // Slow wobble
		if (wobbleTime > 2 * M_PI) {
			wobbleTime -= 2 * M_PI;
		}

		// Calculate subtle position offsets using different frequencies
		offsetX = wobbleRadius * sin(wobbleTime * 0.7f);
		offsetY = wobbleRadius * cos(wobbleTime * 0.5f);

		// Update size animation
		sizeTime += sizeSpeed * deltaTime;
		if (sizeTime > 2 * M_PI) {
			sizeTime -= 2 * M_PI;
		}
		size = 1.0f + 0.5f * sin(sizeTime); // Fluctuate size between 1.0 and 1.5

		if (isSupernova) {
			supernovaTime += deltaTime;
			if (supernovaTime >= supernovaDuration) {
				isSupernova = false; // Reset to regular star
				supernovaTime = 0.0f;
				size = 1.0f; // Reset size
				brightness = 0.5f; // Reset brightness to default twinkle
			}
		}
		else if (rand() % 6000 < 1) { // Random chance to become supernova
			isSupernova = true;
			supernovaTime = 0.0f; // Reset timer
			brightness = 1.0f;    // Full brightness for supernova
			size = 2.0f;          // Larger size for supernova
		}

		// Loop stars back to the other side when they drift out of bounds
		if (getRenderX() < -800.0f) x += 1600.0f; // Screen width
		if (getRenderX() > 800.0f) x -= 1600.0f;  // Screen width
		if (getRenderY() < -450.0f) y += 900.0f;  // Screen height
		if (getRenderY() > 450.0f) y -= 900.0f;   // Screen height
	}

	/*!************************************************************************
	 \brief
	  Computes the star's current x-coordinate for rendering.

	 \return
	  The x-coordinate including base position, wobble offset, and global drift.

	 \details
	  Uses sine for wobble calculations and adds the drift offset.
	**************************************************************************/
	float getRenderX() const { return x + offsetX + driftX; }

	/*!************************************************************************
	 \brief
	  Computes the star's current y-coordinate for rendering.

	 \return
	  The y-coordinate including base position, wobble offset, and global drift.

	 \details
	  Uses cosine for wobble calculations and adds the drift offset.
	**************************************************************************/
	float getRenderY() const { return y + offsetY + driftY; }

};

/*!************************************************************************
\brief
	Initializes all star objects.

\details
	This function sets up the star field by creating and positioning each star.
**************************************************************************/
void InitializeStars();

/*!************************************************************************
\brief
	Updates all star objects.

\param deltaTime
	The time elapsed since the last update (in seconds).

\details
	Iterates through the star field and updates each star's properties based on
	the elapsed time.
**************************************************************************/
void UpdateStars(float deltaTime);

/*!************************************************************************
\brief
	Renders all star objects to the screen.

\param regularStarMesh
	Mesh data for rendering regular stars.

\param supernovaStarMesh
	Mesh data for rendering stars in a supernova state.

\details
	Draws both regular stars and supernova stars using the provided mesh data.
**************************************************************************/
void RenderStars(AEGfxVertexList* regularStarMesh, AEGfxVertexList* supernovaStarMesh);