/******************************************************************************/
/**
 * @file        Player.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (Primary) - 90%
 * @author      Guok Yi Yong (Secondary) - 10%
 * @brief       Handles player-related logic including health, position, and rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"
class Camera;

const float VENUS_CIRCLE_SCALE = 400.0f;
const float EARTH_CIRCLE_SCALE = 500.0f;
const float MAX_HEALTH = 5500.0f; // 5500.0f
const float HEALTH_BAR_WIDTH = 500.0f;

extern float EarthHealth;
extern float health;
extern float playerX;
extern float playerY;

extern AEMtx33 TransformCircle[3];
extern AEMtx33 TransformHealthRect;
extern AEVec2 EarthPosition;

void EarthInit();
void EarthHealthBar(float Health);
void EarthRender();
void ResetEarthHealth();