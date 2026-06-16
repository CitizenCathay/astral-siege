/******************************************************************************/
/**
 * @file        UIManager.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       Manages user interface elements such as fonts, screen coordinates,
 *              and rendering UI elements.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <functional>
#include <vector>
#include <fstream>
#include <string>
#include "AEEngine.h"
#include "AEGraphics.h"

struct UILayoutSettings {
	f32 leftMargin = -0.95f;
	f32 topPosition = 0.70f;
	f32 lineSpacingFactor = 2.0f;
};

class UIManager {
private:
	static s8 fontId;
	static s8 titleFont;
	static UILayoutSettings layoutSettings; // Store UI layout settings
	static bool fontsInitialized;

public:
	UIManager();
	~UIManager(); // Destructor

	// font initialization and cleanup
	static void InitializeFonts();
	static void DestroyFonts();

	// common ui functions
	void DrawTitle(const char* title, float scale = 1.5f);
	void DrawLevelIndicator(int level);  //Show which level

	// screen coordinates conversion helpers
	static void ScreenToNormalized(int screenX, int screenY, float& normX, float& normY);
	static std::pair<float, float> GetNormalizedCursor();

	// getters for accessing fonts
	static s8 GetFontId() { return fontId; }
	static s8 GetTitleFont() { return titleFont; }

	// UI layout settings methods
	static void SetLayoutSettings(const UILayoutSettings& settings) { layoutSettings = settings; }
	static const UILayoutSettings& GetLayoutSettings() { return layoutSettings; }
};

extern UIManager* uiManager;