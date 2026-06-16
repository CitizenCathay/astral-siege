/******************************************************************************/
/**
 * @file        GameOver.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Manages the game-over screen and related logic
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"

static const float screenHeight = 900.0f;
static const char* gameOverButtonLabels[] = { "QUIT GAME", "RESTART", "MAIN MENU" };
static const int gameOverButtonCount = 3;
static const float gameOverVerticalSpacing = 90.0f / screenHeight;

void GameOver_Load() {
    uiManager = new UIManager();
    uiManager->InitializeFonts();
}

void GameOver_Initialize() {
    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();

    // Create star meshes first
    starMesh = createCircleMesh(0xFFFFFFFF);
    supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

    InitializeStars(); // Initialize the star field

    // Store current volume settings
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    // Play defeat sound
    AudioPlayer::GetInstance().PlayDefeat();

    // Make sure volume settings are properly applied
    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);  // SFX
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume); // BGM

    ShowQuitConfirmation = false;
}

void GameOver_Update() {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    UpdateStars(dt); // Update star positions

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        auto cursor = UIManager::GetNormalizedCursor();

        for (int i = 0; i < gameOverButtonCount; i++) {
            float textWidth, textHeight;
            AEGfxGetPrintSize(UIManager::GetFontId(), gameOverButtonLabels[i], 1.0f, &textWidth, &textHeight);

            float centerY = -textHeight / 2.0f;
            float y = centerY + (i * gameOverVerticalSpacing); // center vertically
            float leftX = -textWidth / 2.0f;
            float rightX = leftX + textWidth;
            float topY = y + (textHeight * 2.0f);
            float bottomY = y;

            if (cursor.first >= leftX && cursor.first <= rightX &&
                cursor.second >= bottomY && cursor.second <= topY) {
                GameOverButtonActions[i]();
                break;
            }
        }
    }

    if (ShowQuitConfirmation) {
        PopUpQuit_Update();
    }
}

void GameOver_Draw() {
    AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);
    RenderStars(starMesh, supernovaStarMesh);

    // Draw the title "CREDITS" at the top
    uiManager->DrawTitle("Game Over..");

    for (int i = 0; i < gameOverButtonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), gameOverButtonLabels[i], 1.0f, &textWidth, &textHeight);

        float leftX = -textWidth / 2.0f;
        float centerY = -textHeight / 2.0f;
        float y = centerY + (i * gameOverVerticalSpacing);

        AEGfxPrint(UIManager::GetFontId(), gameOverButtonLabels[i], leftX, y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    if (ShowQuitConfirmation) {
        PopUpQuit_Draw();
    }
}

void GameOver_Free() {
    MeshManager::GetInstance().UnloadTexture();
    MeshManager::GetInstance().UnloadMesh();

    AEGfxMeshFree(starMesh);
    AEGfxMeshFree(supernovaStarMesh);
}

void GameOver_Unload() {
    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;
}