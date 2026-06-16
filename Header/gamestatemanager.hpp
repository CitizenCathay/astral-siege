/******************************************************************************/
/**
 * @file        GameStateManager.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       This file manages transitions between different game states.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "../Header/gamestatelist.hpp"
// ----------------------------------------------------------------------------
// Declaring Variables
// ----------------------------------------------------------------------------
typedef void(*FP)(void);
extern int current, previous, next;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

// ----------------------------------------------------------------------------
// Initialises the Game State Manager functions
// It manages transitions between game states and assigns function pointers
// ----------------------------------------------------------------------------
void GSM_Initialize(int startingState);

// ----------------------------------------------------------------------------
// It manages transitions between game states and assigns function pointers eg.
// Load, Initalise, Update and etc
// ----------------------------------------------------------------------------
void GSM_Update();