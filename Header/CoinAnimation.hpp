/******************************************************************************/
/**
 * @file        CoinAnimation.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file handles the coin animation, including loading, updating,
 *              and rendering of coin frames
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"
#include "AEGraphics.h"

class CoinAnimation {
private:
	AEGfxVertexList* coinMesh;
	AEGfxTexture* coinTexture;

	float animTimer;
	int currentFrame;
	int totalFrame;
	float frameDuration; // how long each frame lasts

public:
	CoinAnimation();
	~CoinAnimation();

	void Initialize();
	void Update();
	void DrawCurrencyText(float x, float y, float scale, int currencyValue);
	void DrawCoinOnly(float x, float y, float scale);
};