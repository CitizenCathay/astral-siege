/******************************************************************************/
/**
 * @file        EnemyFactory.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Creates enemy instances based on configuration data.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/EnemyFactory.hpp"
#include "../Header/Camera.hpp"
#include "../Header/Enemy.hpp"
#include <cstdlib>
#include <cmath>
#include <string>
#include "../Header/MeshManager.hpp"

// Forward declarations for behavior functions.
// Movement functions
extern void defaultMoveBehavior(Enemy&, float);
extern void fastMoveBehavior(Enemy&, float);
extern void dodgeMoveBehavior(Enemy&, float);

// Attack style functions
extern void defaultShootBehavior(Enemy&, float, const AEVec2&);
extern void spreadShotBehavior(Enemy& enemy, float dt, const AEVec2& EarthPosition);

// Constants should match those in enemy.cpp.
static const float OFF_SCREEN_X = -800.0f * cameraZoom;
static const float TARGET_MIN_X = -750.0f * cameraZoom;
static const float TARGET_MAX_X = -300.0f * cameraZoom;
static const float POSITION_THRESHOLD = 5.0f;
static const float ENEMY_SPEED = 80.0f * cameraZoom;
static const float DRIFT_RADIUS = 20.0f;
static const float DRIFT_FREQUENCY = 0.5f;
static const float LERP_ALPHA = 0.1f;
static const float SHOOT_INTERVAL = 2.0f;
static const int MIN_TARGET_DISTANCE = 15;  // Use same value as in enemy.cpp

// Helper function: Check if a target position is too close to any other enemy targets.
extern bool isTooClose(float newX, float newY);

/*!************************************************************************
\brief
    Retrieves the movement behavior function pointer based on the behavior name.

\param behavior
    A string representing the movement behavior type.

\return
    A function pointer to the corresponding movement behavior function.

\details
    Currently, both "default" and "fast" behaviors use the same function
    (`defaultMoveBehavior`). Additional behaviors can be implemented later.
**************************************************************************/
static void (*GetMoveBehavior(const std::string& behavior))(Enemy&, float) {
    if (behavior == "default") return defaultMoveBehavior;
    if (behavior == "fast") return fastMoveBehavior;
    if (behavior == "dodge") return dodgeMoveBehavior;
    return defaultMoveBehavior; // Fallback.
}

/*!************************************************************************
\brief
    Retrieves the shooting behavior function pointer based on the behavior name.

\param behavior
    A string representing the shooting behavior type.

\return
    A function pointer to the corresponding shooting behavior function.

\details
    Currently, both "default" and "rapid" behaviors use the same function
    (`defaultShootBehavior`). Additional behaviors can be implemented later.
**************************************************************************/
static void (*GetShootBehavior(const std::string& behavior))(Enemy&, float, const AEVec2&) {
    if (behavior == "default") {
        return defaultShootBehavior;
    }
    if (behavior == "spread") {
        return spreadShotBehavior;
    }
    return defaultShootBehavior; // Fallback.
}

/*!************************************************************************
\brief
    Creates an enemy instance using a configuration struct.

\param config
    A reference to an `EnemyConfig` struct containing enemy properties.

\return
    An initialized `Enemy` object with assigned properties.

\details
    This function initializes an enemy with attributes specified in the given `config`.
    The enemy is spawned at a randomized off-screen position and is assigned movement
    and shooting behaviors based on the configuration. A unique target position is also
    generated for the enemy to move toward.
**************************************************************************/
Enemy EnemyFactory::CreateEnemy(const EnemyConfig& config) {
    // Get current camera.
    Camera* camera = CameraManager::GetInstance().GetCurrentCamera();

    // Define an extra margin to ensure the enemy is off-screen.
    const float extraMargin = 25.0f;

    // Compute the left edge of the camera's view in world coordinates.
    // Assuming camera->position.x is the camera center and camera->cameraZoom is its zoom factor
    float leftEdge = camera->position.x - (AEGfxGetWindowWidth() / 2.0f) / camera->cameraZoom;

    // Set spawn position off-screen (to the left) relative to the current camera view.
    float spawnX = leftEdge - extraMargin;

    Enemy enemy;

    // Set enemy stats from config.
    enemy.health = config.health;
    enemy.speed = config.speed;
    enemy.shootInterval = config.shootInterval;
    enemy.mesh = config.mesh; // How you handle mesh lookup is up to you (Chloe/Celeste)
    enemy.currencyReward = config.currencyReward; // for currency

    if (config.type == "base_enemy") {
        enemy.textureType = TextureType::ENEMY1;
    }
    else if (config.type == "base_enemy_fast") {
        enemy.textureType = TextureType::ENEMY3;
    }
    else if (config.type == "base_enemy_dodge") {
        enemy.textureType = TextureType::ENEMY2;
    }
    else if (config.type == "spread_shooter_default" ||
        config.type == "spread_shooter_dodge") {
        enemy.textureType = TextureType::ENEMY4;
    }
    else {
        enemy.textureType = TextureType::ENEMY1; // Default fallback
    }

    // Set randomized spawning position (off-screen).
    enemy.Position.x = spawnX;
    enemy.Position.y = static_cast<float>((rand() % 600) - 300);

    // Mark enemy as active and initialize shoot timer.
    enemy.In_Game = true;
    enemy.ShootTimer = static_cast<float>(rand() % 2000) / 1000.0f;

    // Randomize a target position for this enemy using the helper function.
    enemy.targetPosition = GenerateTargetPosition();

    // Initialize hovering variables.
    enemy.hovering = false;
    enemy.hoverTime = 0.0f;
    enemy.hoverPhase = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 6.28318530718f;

    // Assign behavior function pointers based on config.
    enemy.moveBehavior = GetMoveBehavior(config.moveBehavior);
    enemy.shootBehavior = GetShootBehavior(config.shootBehavior);

    return enemy;
}