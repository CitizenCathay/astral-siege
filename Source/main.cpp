/******************************************************************************/
/**
 * @file        Main.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Entry point of the application. Initializes the Alpha Engine, manages game state transitions,
 *              and handles the main game loop.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/


#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#include <crtdbg.h> // To check for memory leaks
#include <cmath>
#include <iostream>
#include "AEEngine.h"
#include "../Header/Game.hpp"

//==================================================
// Main Application
//==================================================
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize the system
    AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);
    AESysSetWindowTitle("Astral Siege");

    // Start directly in the Game State
    GSM_Initialize(GS_SPLASHSCREEN);

    // Main game loop
    while (current != GS_QUIT) {
        if (current != GS_RESTART) {
            GSM_Update();    // Update Game State Manager
            fpLoad();        // Load resources
        }
        else {
            next = previous;  // Restart: revert to the previous state
            current = previous;
        }

        fpInitialize();  // Initialize game state

        // Game frame loop
        while (next == current) {
            AESysFrameStart();  // Start the AE frame
            fpUpdate();         // Update game logic
            fpDraw();           // Render the game
            AESysFrameEnd();    // End the AE frame
            if (!AESysDoesWindowExist()) {
                next = GS_QUIT; // Force quit if the window is closed
            }
        }

        fpFree();  // Free temporary resources

        if (next != GS_RESTART) {
            fpUnload();  // Unload resources if not restarting
        }

        // Move to the next game state
        previous = current;
        current = next;
    }

    // Clean up Alpha Engine resources and audio before exiting program
    if (!nullptr) {
        AudioManager::GetInstance().StopSFXOnly();
        AudioManager::GetInstance().StopAll();
        AudioPlayer::DestroyInstance();
        AudioManager::DestroyInstance();
    }
    AESysExit();
    return 0;
}
