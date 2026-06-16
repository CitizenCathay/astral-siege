/******************************************************************************/
/**
 * @file        TransitionManager.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Handles screen transitions and button navigation.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"
#include "../Header/OuterPlanetManager.hpp"
#include <iostream>

extern int currentLevel;
// Maps button index to corresponding function pointer (0 to 6) (MainMenu.cpp)
void (*ButtonActions[])() = {
    StartGame, Credits, Controls, Options, Quit
};

void StartGame() {
    ResetGameState(CameraManager::GetInstance().GetCurrentCamera());
    next = GS_GAME; // Set the next state to the game state
}

void Credits() {
    // Implement show credits logic
    next = GS_CREDITS;
}

void Controls() {
    // Implement show controls logic
    next = GS_CONTROLS;
}

void Options() {
    next = GS_OPTIONS;
    // Implement show options logic
}

void Quit() {
    ShowQuitConfirmation = true;
}



// GameOver.Cpp & GameWin.cpp
void (*GameOverButtonActions[])() = {
    QuitGame, Restart, BackMainMenu, LoadNextLevel
};

void Restart() {
    // Store volumes before resetting
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    CurrencyManager::GetInstance().Reset();
    ResetGameState(CameraManager::GetInstance().GetCurrentCamera());

    // Restore volumes after reset
    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume);

    CurrencyManager::GetInstance().Reset();
    ResetGameState(CameraManager::GetInstance().GetCurrentCamera());

    next = GS_GAME;
}

void BackMainMenu() {
    // Store volumes before resetting
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    // Restore volumes after reset
    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume);

    next = GS_MAINMENU;
}

void QuitGame() {
    ShowQuitConfirmation = true;
}

void LoadNextLevel() {
   IncrementLevel(); // Increase level count
   ResetGameLevel(CameraManager::GetInstance().GetCurrentCamera());
   next = GS_GAME;
}
