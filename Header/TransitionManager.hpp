/******************************************************************************/
/**
 * @file        TransitionManager.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Manages transitions between different game states and button actions.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
// Function for button actions
void StartGame();
void Controls();
void Options();
void Credits();
void Quit();

// Declare the number of buttons
enum ButtonAction {
    START_GAME,
    LOAD_SAVED_GAME,
    CONTROLS,
    OPTIONS,
    CREDITS,
    EXTRAS,
    QUIT,
    NUM_BUTTONS
};

// Array of function pointers for button actions
extern void (*ButtonActions[NUM_BUTTONS])();


void Restart();
void BackMainMenu();
void LoadNextLevel(); // Function to load the next level
void QuitGame();

enum GameOverButtonAction {
    RESTART_GAME,
    BACK_TO_MAIN_MENU,
    QUIT_GAME,
    NEXT_LEVEL,
    GAMEOVER_NUM_BUTTONS
};

extern void (*GameOverButtonActions[GAMEOVER_NUM_BUTTONS])();