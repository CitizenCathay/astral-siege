/******************************************************************************/
/**
 * @file        HUD.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       This file handles the Heads-Up Display (HUD) for the game, including
 *              shop, zoom, weapon status, and shield indicators.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#ifndef HUD_HEADER
#define HUD_HEADER
#include "GameObject.hpp"

enum HUDUI {
	SHOPUI,
	ZOOMUI,
	WEAPONUI,
	TURR1UI,
	TURR2UI,
	SHIELDUI,
	SELLUI,
	TOTALUI
};

class HUD {
public:
	HUD(); 
	void Init();
	void Update();
	void Draw();
	void Reset();
private:
	int zoomCount;
	std::vector<GameObject> hudItems;
	bool hoverFlags[TOTALUI];
	bool hasClick;
	s8 fontId;
	f32 screenWidth;
	f32 screenHeight;
};




#endif HUD_HEADER