
/* Start Header ************************************************************************/
/*
 * @file        UIManager.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (Primary) - Game mechanics, physics implementation, overall architecture (85%)
 * @author      Guok Yi Yong (Secondary) - Add DrawLevelIndicator to give player a better gameplay expereinces (10%)
 * @author      Celeste Tong Jia Xuan (Secondary) - Added GetNormalizedCursor (5%)
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore. All
 *               rights reserved.
 */
 /* End Header **************************************************************************/


#include "../Header/UIManager.hpp"
#include "AEGraphics.h"
#include "AEEngine.h"
#include <cmath>
#include <string>
#include <fstream>

// initiate static members
s8 UIManager::fontId = 1;
s8 UIManager::titleFont = 2;
UILayoutSettings UIManager::layoutSettings;

bool UIManager::fontsInitialized = false;
static bool fontSystemInitialized = false;

static const float WindowWidth = 1600.0f;
static const float WindowHeight = 900.0f;
extern int enemiesDestroyed;
extern int counter;
UIManager::UIManager() {}
UIManager::~UIManager() {} // Destructor

// font initialization and cleanup
void UIManager::InitializeFonts() {
    AEGfxFontSystemStart();
    fontId = AEGfxCreateFont("Assets/Fonts/ChakraPetch-Bold.ttf", 30);
    titleFont = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 40);
}

void UIManager::DestroyFonts() {
    if (!fontsInitialized) return;

    if (fontId > 0) {
        AEGfxDestroyFont(fontId);
        fontId = -1;
    }

    if (titleFont > 0) {
        AEGfxDestroyFont(titleFont);
        titleFont = -1;
    }

    fontsInitialized = false;
}

// common ui functions
void UIManager::DrawTitle(const char* title, float scale) {
	f32 targetY = 0.8f;
	f32 titleWidth = 0, titleHeight = 0;
	AEGfxGetPrintSize(titleFont, title, 1.5f, &titleWidth, &titleHeight);

	float titleColor[4] = { 1.0f, 0.84f, 0.0f, 1.0f }; // Use Gold for title

	AEGfxPrint(titleFont, title, -titleWidth / 2, targetY - titleHeight / 2,
		scale, titleColor[0], titleColor[1], titleColor[2], titleColor[3]);
}

// screen coordinates conversion helpers
void UIManager::ScreenToNormalized(int screenX, int screenY, float& normX, float& normY) {
	normX = ((float)screenX / (WindowWidth / 2)) - 1.0f;						// Convert mouse X position to normalized space (-1 to 1)
	normY = ((float)(WindowHeight - screenY) / (WindowHeight / 2)) - 1.0f;
}

void UIManager::DrawLevelIndicator(int level) {
    char levelText[32];
    char enemyText[32];
    char winConditionText[64]; // Added for win condition display

    snprintf(levelText, sizeof(levelText), "Level: %d", level);
    snprintf(enemyText, sizeof(enemyText), "Enemies: %d", enemiesDestroyed);

    // Determine win condition
    int requiredEnemies = (level == 0) ? 10 : (level == 1) ? 15 : 30;
    int requiredCounter = (level == 0) ? 1 : 2;  // Counter required for each level

    // Check if conditions are met
    bool enemiesCleared = enemiesDestroyed >= requiredEnemies;
    bool counterMet = (counter == requiredCounter);

    // Generate win condition text
    if (!enemiesCleared) {
        snprintf(winConditionText, sizeof(winConditionText),
            "Defeat %d more enemies!", requiredEnemies - enemiesDestroyed);
    }
    else if (!counterMet) {
        snprintf(winConditionText, sizeof(winConditionText),
            "Increase your telescope level to %d!", requiredCounter);
    }
    else {
        snprintf(winConditionText, sizeof(winConditionText), "Proceed to the next level!");
    }

    // Get margin and position from layout settings
    f32 leftMargin = layoutSettings.leftMargin;
    f32 topPosition = layoutSettings.topPosition + 0.15f;

    // Calculate text height for spacing
    f32 textHeight = 0;
    AEGfxGetPrintSize(fontId, levelText, 1.0f, nullptr, &textHeight);
    f32 lineSpacing = textHeight * layoutSettings.lineSpacingFactor;

    float textColor[4] = { 1.0f, 1.0f, 1.0f, 0.8f }; // White

    // Draw UI
    AEGfxPrint(fontId, levelText, leftMargin, topPosition,
        1.0f, textColor[0], textColor[1], textColor[2], textColor[3]);

    AEGfxPrint(fontId, enemyText, leftMargin, topPosition - lineSpacing,
        1.0f, textColor[0], textColor[1], textColor[2], textColor[3]);

    AEGfxPrint(fontId, winConditionText, leftMargin, topPosition - (2 * lineSpacing),
        1.0f, textColor[0], textColor[1], textColor[2], textColor[3]);
}

std::pair<float, float> UIManager::GetNormalizedCursor() {
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    float normX, normY;
    UIManager::ScreenToNormalized(mouseX, mouseY, normX, normY);

    return { normX, normY };
}
