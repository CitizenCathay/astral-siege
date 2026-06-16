/******************************************************************************/
/**
 * @file        MainMenu.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file handles the main menu screen, including user interaction
 *              and button selection.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"

void Menu_Load();
void Menu_Initialize();
void Menu_Update();
void Menu_Draw();
void Menu_Free();
void Menu_Unload();

// Linear interpolation function for smooth scaling
float Lerp(float current, float target, float speed, float dt);