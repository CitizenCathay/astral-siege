/******************************************************************************/
/**
 * @file        CoinAnimation.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       Implementation of the CoinAnimation class, which handles coin animation
 *              and frame updates in the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/CoinAnimation.hpp"
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/UIManager.hpp"
#include <iostream>

CoinAnimation::CoinAnimation() {
	coinMesh = nullptr;
	coinTexture = nullptr;
	animTimer = 0.0f;
	currentFrame = 0;
	totalFrame = 8;
	frameDuration = 0.1f;
}

CoinAnimation::~CoinAnimation() {
	if (coinMesh) {
		AEGfxMeshFree(coinMesh);
		coinMesh = nullptr;
	}
	if (coinTexture) {
		AEGfxTextureUnload(coinTexture);
		coinTexture = nullptr;
	}
}

void CoinAnimation::Initialize() {
	// Create a square mesh for a SINGLE COIN
	AEGfxMeshStart();

	// We'll use UV coordinates that match a single frame
	// The full texture width is divided into totalFrame segments
	float frameWidth = 1.0f / totalFrame;

	// First triangle - Using only the first frame initially
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,                // bottom left
		0.5f, -0.5f, 0xFFFFFFFF, frameWidth, 1.0f,           // bottom right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f                  // top left
	);

	// Second triangle
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, frameWidth, 1.0f,           // bottom right
		0.5f, 0.5f, 0xFFFFFFFF, frameWidth, 0.0f,            // top right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f                  // top left
	);

	coinMesh = AEGfxMeshEnd();

	// Load coin texture
	coinTexture = AEGfxTextureLoad("Assets/currencyIcon.png");
}

void CoinAnimation::Update() {
	// Update animation timer
	animTimer += static_cast<float>(AEFrameRateControllerGetFrameTime());

	// Update to next frame when timer exceeds frame duration
	if (animTimer >= frameDuration) {
		animTimer = 0.0f;
		currentFrame = (currentFrame + 1) % totalFrame;
	}
}

void CoinAnimation::DrawCurrencyText(float posX, float posY, float scale, int currencyValue) {
	// Calculate texture coordinates for current frame
	float frameWidth = 1.0f / totalFrame;
	float frameStartX = currentFrame * frameWidth;

	// Set render properties
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set texture with proper offset for current animation frame
	AEGfxTextureSet(coinTexture, frameStartX, 0.0f);

	// Scale down to reasonable size
	AEMtx33 scaleMat = { 0 };
	AEMtx33Scale(&scaleMat, scale, scale);

	AEMtx33 transMat = { 0 };
	AEMtx33Trans(&transMat, posX, posY);

	// Combine matrices
	AEMtx33 finalMat = { 0 };
	AEMtx33Concat(&finalMat, &transMat, &scaleMat);

	// Draw mesh
	AEGfxSetTransform(finalMat.m);
	AEGfxMeshDraw(coinMesh, AE_GFX_MDM_TRIANGLES);

	// Draw currency value
	char buffer[32];
	sprintf_s(buffer, sizeof(buffer), "%d x", currencyValue);

	// Calculate text position relative to the coin position
	// Convert from world coordinates to normalized coordinates for AEGfxPrint
	float textX = (posX / (AEGfxGetWinMaxX() - AEGfxGetWinMinX())) * 2.0f;
	float textY = (posY / (AEGfxGetWinMaxY() - AEGfxGetWinMinY())) * 2.0f;
	
	// Add a small offset so the text appears next to the coin
	textX += 0.05f;
	
	AEGfxPrint(UIManager::GetFontId(), buffer, textX, textY, 1.5f, 1.0f, 1.0f, 0.0f, 1.0f);
}

void CoinAnimation::DrawCoinOnly(float posX, float posY, float scale) {
	if (!coinMesh || !coinTexture) {
		return;
	}

	// Calculate texture coordinates for current frame
	float frameWidth = 1.0f / totalFrame;
	float frameStartX = currentFrame * frameWidth;

	// Set render properties
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set texture with proper offset for current animation frame
	AEGfxTextureSet(coinTexture, frameStartX, 0.0f);

	// Scale matrix
	AEMtx33 scaleMat = { 0 };
	AEMtx33Scale(&scaleMat, scale, scale);

	// Translation matrix
	AEMtx33 transMat = { 0 };
	AEMtx33Trans(&transMat, posX, posY);

	// Combine matrices
	AEMtx33 finalMat = { 0 };
	AEMtx33Concat(&finalMat, &transMat, &scaleMat);

	// Draw mesh
	AEGfxSetTransform(finalMat.m);
	AEGfxMeshDraw(coinMesh, AE_GFX_MDM_TRIANGLES);
}