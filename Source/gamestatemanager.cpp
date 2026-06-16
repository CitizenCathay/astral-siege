/******************************************************************************/
/**
 * @file        GameStateManager.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Handles state changes between game states.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Game.hpp"

// Initialize game state variables as GS_STATES
int current = 0, previous = 0, next = 0;
FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState) {
    current = previous = next = startingState;
}

void GSM_Update() {
    switch (current) {
    case GS_GAME:
        fpLoad = Game_Load;
        fpInitialize = Game_Initialize;
        fpUpdate = Game_Update;
        fpDraw = Game_Draw;
        fpFree = Game_Free;
        fpUnload = Game_Unload;
        break;

    case GS_MAINMENU:
        fpLoad = Menu_Load;
        fpInitialize = Menu_Initialize;
        fpUpdate = Menu_Update;
        fpDraw = Menu_Draw;
        fpFree = Menu_Free;
        fpUnload = Menu_Unload;
        break;

    case GS_SPLASHSCREEN:
        fpLoad = Splash_Load;
        fpInitialize = Splash_Initialize;
        fpUpdate = Splash_Update;
        fpDraw = Splash_Draw;
        fpFree = Splash_Free;
        fpUnload = Splash_Unload;
        break;

    case GS_CONTROLS:
        fpLoad = Tutorial_Load;
        fpInitialize = Tutorial_Initialize;
        fpUpdate = Tutorial_Update;
        fpDraw = Tutorial_Draw;
        fpFree = Tutorial_Free;
        fpUnload = Tutorial_Unload;
        break;

    case GS_CREDITS:
        fpLoad = Credits_Load;
        fpInitialize = Credits_Initialize;
        fpUpdate = Credits_Update;
        fpDraw = Credits_Draw;
        fpFree = Credits_Free;
        fpUnload = Credits_Unload;
        break;

    case GS_OPTIONS:
        fpLoad = Options_Load;
        fpInitialize = Options_Initialize;
        fpUpdate = Options_Update;
        fpDraw = Options_Draw;
        fpFree = Options_Free;
        fpUnload = Options_Unload;
        break;

    case GS_GAMEOVER:
        fpLoad = GameOver_Load;
        fpInitialize = GameOver_Initialize;
        fpUpdate = GameOver_Update;
        fpDraw = GameOver_Draw;
        fpFree = GameOver_Free;
        fpUnload = GameOver_Unload;
        break;

    case GS_GAMEWIN:
        fpLoad = GameWin_Load;
        fpInitialize = GameWin_Initialize;
        fpUpdate = GameWin_Update;
        fpDraw = GameWin_Draw;
        fpFree = GameWin_Free;
        fpUnload = GameWin_Unload;
        break;

    case GS_RESTART:
        break;

    case GS_QUIT:
        // Exit handled in main loop
        break;

    default:
        break;
    }
}