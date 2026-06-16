/******************************************************************************/
/**
 * @file        EnemyBullet.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       This file manages enemy bullets, including creation, movement,
 *              rendering, and collision detection.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"

constexpr auto MAX_BULLETS = 300;  // Maximum bullets at a time;
constexpr auto BULLET_SCALE = 5.0f;
struct BulletEnemy {
    AEVec2 Position;    // Bullet's position
    AEVec2 Velocity;    // Bullet's velocity (direction & speed)
    bool In_Game;       // Is bullet active?
};

void BulletCreate(float StartX, float StartY, float VelocityX, float VelocityY);

void BulletMovement(float dt);                      // Update bullet positions

void BulletRender();    // Draw bullets

void CheckBulletCollisions(float& EarthHealth, AEVec2& EarthPosition, float EarthRadius);  // Detect collisions with Earth

void ResetBullets();

BulletEnemy* GetEnemyBullet();