/******************************************************************************/
/**
 * @file        Enemy.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This file manages enemy entities, including movement, shooting,
 *              and game state updates.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "../Header/MeshManager.hpp"
#include "AEEngine.h"
#include <string>

constexpr auto MAX_ENEMY = 1000;

class Camera;

/*!*************************************************************************
\struct Enemy
\brief
    Represents an enemy entity in the game.

\details
    The `Enemy` structure contains information related to enemy attributes,
    movement, shooting behavior, and in-game state. It supports function
    pointers to dynamically assign movement and shooting behaviors.

***************************************************************************/
struct Enemy {
    // Position and movement
    AEVec2 Position;         // Current position.
    AEVec2 targetPosition;   // Target position to move toward.
    AEVec2 Velocity;         // Direction and speed vector.

    // Game state
    bool In_Game;            // Is the enemy active in the game?
    float ShootTimer;        // Timer to track shooting intervals.

    // Additional enemy stats
    int health;              // Enemy health.
    float speed;             // Movement speed.
    float shootInterval;     // Time interval between shots.
    std::string mesh;        // Identifier for the enemy's mesh.

    // Hovering behavior
    bool hovering;           // Has the enemy reached its target and started hovering?
    float hoverTime;         // How long the enemy has been hovering.
    float hoverPhase;        // Random phase offset (to avoid synchronized movement).

    float dodgeTimer;  // Timer for deciding when to move again
    TextureType textureType;

    //==================================================
    // Function pointers for behavior.
    //==================================================

    /*!*************************************************************************
    \brief Pointer to the function handling the enemy's movement behavior.

    \details
        This function pointer allows dynamic assignment of movement behavior,
        enabling different enemy types to have unique movement patterns.
    ***************************************************************************/
    void (*moveBehavior)(Enemy&, float);

    /*!*************************************************************************
    \brief Pointer to the function handling the enemy's shooting behavior.

    \details
        This function pointer allows dynamic assignment of shooting behavior,
        enabling different enemy types to fire projectiles in unique ways.

    \param Enemy& Reference to the enemy performing the action.
    \param float Time elapsed (delta time).
    \param AEVec2& Position of the target (Earth).
    ***************************************************************************/
    void (*shootBehavior)(Enemy&, float, const AEVec2&);

    int currencyReward;  // Amount of currency this enemy drops when destroyed
    void KillEnemy();
    Enemy();
};

/*!*************************************************************************
\brief
    Retrieves the global list of enemy entities.

\return
    A pointer to the global array of `Enemy` objects.

\details
    This function returns a pointer to the global enemy list, allowing other
    parts of the game to access and manage enemy instances.
***************************************************************************/
Enemy* GetEnemyList();

/*!*************************************************************************
\brief
    Updates all enemies in the game, handling movement, hovering, and shooting.

\param dt
    Delta time (time elapsed since the last frame).

\param EarthPosition
    The current position of the Earth (enemy target).

\param EarthHealth
    Reference to Earth's health, which may be affected by enemy attacks.

\details
    This function iterates through all active enemies, updating their movement,
    hovering state, and shooting behavior. It also handles collision detection
    and applies damage to Earth if an enemy reaches it.
***************************************************************************/
void EnemyMovement(float dt, AEVec2& EarthPosition, float& EarthHealth);

/*!*************************************************************************
\brief
    Renders all active enemies.

\details
    This function applies transformations (positioning and scaling) and
    renders each active enemy on the screen.
***************************************************************************/
void EnemyRender();

/*!*************************************************************************
\brief
    Fires a projectile burst toward a target.

\param OriginalX
    X-coordinate of the enemy firing.

\param OriginalY
    Y-coordinate of the enemy firing.

\param NewX
    X-coordinate of the target.

\param NewY
    Y-coordinate of the target.

\details
    Computes a trajectory from the enemy to the target and spawns multiple
    projectiles, forming a burst aimed at the target position.
***************************************************************************/
void EnemyShoot(float OriginalX, float OriginalY, float NewX, float NewY);

/*!*************************************************************************
\brief
    Resets all enemies when restarting the level.

\details
    Iterates through the global enemy list, deactivating all enemies and
    resetting their shooting timers and positions. This function ensures a
    clean state when the game restarts.
***************************************************************************/
void ResetEnemies();

/*!*************************************************************************
\brief
    Determines whether a given position is too close to existing enemies.

\param newX
    The X-coordinate of the new position.

\param newY
    The Y-coordinate of the new position.

\return
    `true` if the position is too close to an existing enemy, `false` otherwise.

\details
    Ensures that newly spawned enemies do not overlap with existing ones,
    helping to maintain balanced enemy distribution.
***************************************************************************/
bool isTooClose(float newX, float newY);

AEVec2 GenerateTargetPosition();

// Legacy function/fallback
// void EnemyCreate();