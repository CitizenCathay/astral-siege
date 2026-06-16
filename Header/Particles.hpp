/******************************************************************************/
/**
 * @file        Particles.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Manages particle and explosion effects.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <vector>
#include "../Header/Vector.hpp"
#include "AEEngine.h"

class Camera;

struct Particle {
	float x, y;       // Position
	float vx, vy;     // Velocity
	float ax, ay;     // Acceleration
	float lifetime;   // Remaining lifetime
};

struct Explosion {
	float x, y;         // Position of the explosion
	float size;         // Size of the explosion
	float lifetime;     // Remaining lifetime of the explosion
	float shakeIntensity = 1.0f; // Maximum shake displacement
	float shakeOffsetX = 0.0f;  // Add shake offsets
	float shakeOffsetY = 0.0f;

	Explosion() :
		x(0.0f),
		y(0.0f),
		size(0.0f),
		lifetime(0.0f)
	{

	}
};

bool IsInsidePlanet(float x, float y, const std::vector<GravityPoint>& gravityPoints);

void SpawnExplosion(std::vector<Explosion>& explosions, float x, float y);

void UpdateExplosions(std::vector<Explosion>& explosions, float deltaTime);

void RenderExplosions(const std::vector<Explosion>& explosions, AEGfxVertexList* explosionMesh, Camera* camera);

void SpawnParticles(std::vector<Particle>& particles, int count, bool particlesEnable, const std::vector<GravityPoint>& gravityPoints);

void UpdateParticles(std::vector<Particle>& particles,
	std::vector<VectorField>& field,
	float deltaTime,
	const std::vector<GravityPoint>& gravityPoints,
	std::vector<Explosion>& explosions);

void RenderParticles(std::vector<Particle>& particles, AEGfxVertexList* particleMesh, Camera* camera);
