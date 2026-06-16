/******************************************************************************/
/**
 * @file        GameWin.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Manages the game-win screen and related logic
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
                All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"

static const float screenHeight = 900.0f;
static const char* winButtonLabels[] = { "QUIT GAME", "RESTART", "MAIN MENU" };
static const int winButtonCount = 3;
static const float winVerticalSpacing = 90.0f / screenHeight;

void GameWin_Load() {
    uiManager = new UIManager();
    uiManager->InitializeFonts();
}

void GameWin_Initialize() {
    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();

    // Create star meshes first
    starMesh = createCircleMesh(0xFFFFFFFF);
    supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

    InitializeStars(); // Initialize the star field

    // Make sure audio is properly initialized before playing sounds
    AudioManager::GetInstance().InitializeDefaultAudio(false); // Initialize audio without BGM
    AudioPlayer::GetInstance();

    // Play defeat sound when game over screen initializes
    AudioPlayer::GetInstance().PlayVictory();
    ShowQuitConfirmation = false;
}

void GameWin_Update() {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    UpdateStars(dt); // Update star positions

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        auto cursor = UIManager::GetNormalizedCursor();

        for (int i = 0; i < winButtonCount; ++i) {
            float textWidth, textHeight;
            AEGfxGetPrintSize(UIManager::GetFontId(), winButtonLabels[i], 1.0f, &textWidth, &textHeight);

            float leftX = -textWidth / 2.0f;
            float rightX = leftX + textWidth;
            float centerY = -textHeight / 2.0f;
            float topy = centerY + (i * winVerticalSpacing);
            float bottomY = topy + textHeight;

            if (cursor.first >= leftX && cursor.first <= rightX &&
                cursor.second >= topy && cursor.second <= bottomY) {
                GameOverButtonActions[i](); // Execute action
                break;
            }
        }
    }
    if (ShowQuitConfirmation) {
        PopUpQuit_Update();
    }
}

void GameWin_Draw() {
    AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);
    RenderStars(starMesh, supernovaStarMesh);

    uiManager->DrawTitle("Congratulations!");

    for (int i = 0; i < winButtonCount; ++i) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), winButtonLabels[i], 1.0f, &textWidth, &textHeight);

        float leftX = -textWidth / 2.0f;
        float centerY = -textHeight / 2.0f;
        float y = centerY + (i * winVerticalSpacing);

        AEGfxPrint(UIManager::GetFontId(), winButtonLabels[i], leftX, y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    if (ShowQuitConfirmation) {
        PopUpQuit_Draw();
    }
}


void GameWin_Free() {
    MeshManager::GetInstance().UnloadTexture();
    MeshManager::GetInstance().UnloadMesh();

    AEGfxMeshFree(starMesh);
    AEGfxMeshFree(supernovaStarMesh);
}

void GameWin_Unload() {
    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;
}