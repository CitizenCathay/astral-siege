/******************************************************************************/
/**
 * @file        SplashScreen.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Manages the splash screen display and logic.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/UIManager.hpp"
#include "../Header/gamestatemanager.hpp"

void Splash_Load();
void Splash_Initialize();
void Splash_Update();
void Splash_Draw();
void Splash_Free();
void Splash_Unload();