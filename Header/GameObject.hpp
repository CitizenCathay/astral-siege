/******************************************************************************/
/**
 * @file        GameObject.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       This file defines the base class for all game objects, handling
 *              position, scale, rotation, and rendering logic.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/Particles.hpp"
#include "../Header/gravitySystem.hpp"

class GameObject {
public:
	AEVec2 pos{ 0,0 };
	AEVec2 scale{ 0,0 };
	float rotation = 0.0; // degree or radian up to you
	AEGfxVertexList* pMesh; // how i look like
	AEGfxTexture* pTex;
	bool isActive;
	GameObject();
	GameObject(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, int tt=-1);
	AEMtx33 GetTransform() const;
	AEMtx33 GetUITransform() const;
	int textureType;
};

bool CheckCollision(const Particle& particle, const GravityPoint& planet, float planetRadius);

AEMtx33 GetStarTransform(float x, float y, float scaleX, float scaleY); // star

AEMtx33 GetTransform(float x, float y, float scaleX, float scaleY);		// General transform
