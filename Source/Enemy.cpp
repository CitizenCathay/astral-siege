/******************************************************************************/
/**
 * @file        Enemy.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Implements enemy behavior, including movement, shooting,
 *              hovering, and interaction with Earth.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Enemy.hpp"
#include "../Header/EnemyBullet.hpp"
#include "../Header/Camera.hpp"
#include "../Header/OuterPlanetManager.hpp"
#include "../Header/MeshManager.hpp"
#include <cmath>

// Constants for enemy behavior and positioning.
static const int MAX_ENEMY_COUNT = MAX_ENEMY;
static const float MIN_TARGET_DISTANCE = 25.0f;
static const float OFF_SCREEN_X = -800.0f * cameraZoom;
static const float TARGET_MIN_X = -750.0f * cameraZoom;
static const float TARGET_MAX_X = -300.0f * cameraZoom;
static const float POSITION_THRESHOLD = 5.0f;
static const float ENEMY_SPEED = 80.0f * cameraZoom;
static const float DRIFT_RADIUS = 20.0f;
static const float DRIFT_FREQUENCY = 0.5f;
static const float LERP_ALPHA = 0.1f;
static const float EARTH_COLLISION_RADIUS_ENEMY = 20.0f;
static const float EARTH_COLLISION_RADIUS_EARTH = 50.0f;
static const float EARTH_DAMAGE = 5.0f;
static const float SHOOT_INTERVAL = 2.0f;
static const int BULLET_BURST_COUNT = 8;
static const float BULLET_SPACING = 8.0f;
static const float BULLET_SPEED = 1300.0f * cameraZoom;
static const float OFFSET_Y = 18.0f;
static const float OFFSET_X = 25.0f;

// Global array to store enemies
static Enemy Enemies[MAX_ENEMY_COUNT];

OuterPlanetManager outerPlanetManager;

Enemy::Enemy()
    : Position{ 0.0f, 0.0f },
    targetPosition{ 0.0f, 0.0f },
    Velocity{ 0.0f, 0.0f },
    In_Game{ false },
    ShootTimer{ 0.0f },
    health{ 0 },
    speed{ 0.0f },
    shootInterval{ 0.0f },
    mesh{ "" },
    hovering{ false },
    hoverTime{ 0.0f },
    hoverPhase{ 0.0f },
    dodgeTimer{ 0.0f },
    moveBehavior{ nullptr },
    shootBehavior{ nullptr },
    currencyReward{ 0 },
    textureType{ TextureType::ENEMY1 }
{
}

/*!************************************************************************
\brief
    Retrieves the global list of enemies.

\return
    Pointer to the global array of `Enemy` objects.

\details
    This function returns a pointer to the global enemy array, allowing
    other parts of the game to access and manipulate enemy objects.
**************************************************************************/
Enemy* GetEnemyList() {
    return Enemies;
}

/*!************************************************************************
\brief
    Checks if a new target position is too close to existing enemies.

\param newX
    X-coordinate of the new position.

\param newY
    Y-coordinate of the new position.

\return
    `true` if the new position is too close to an existing enemy, `false` otherwise.

\details
    This function ensures that newly spawned enemies do not overlap
    with existing ones, maintaining balanced enemy distribution.
**************************************************************************/
bool isTooClose(float newX, float newY) {
    // Check against other enemy positions.
    for (int j = 0; j < MAX_ENEMY_COUNT; j++) {
        if (Enemies[j].In_Game) {
            float dx = Enemies[j].targetPosition.x - newX;
            float dy = Enemies[j].targetPosition.y - newY;
            if (sqrt(dx * dx + dy * dy) < MIN_TARGET_DISTANCE) {
                return true;
            }
        }
    }

    // Check against planet positions.
    std::vector<GravityPoint>& gravityPoints = outerPlanetManager.getGravityPoints();
    for (GravityPoint const& planet : gravityPoints) {
        float dx = planet.x - newX;
        float dy = planet.y - newY;
        // Add extra margin if needed:
        float exclusionRadius = planet.size; 
        if (sqrt(dx * dx + dy * dy) < exclusionRadius) {
            return true;
        }
    }

    return false;
}

AEVec2 GenerateTargetPosition() {
    AEVec2 newTarget = {};
    float randomFactor;
    Camera* camera = CameraManager::GetInstance().GetCurrentCamera(); // Get current camera
    float zoomFactor = camera->cameraZoom; // Get current zoom level

    // Add safety counter to prevent infinite loops
    int attempts = 0;
    const int MAX_ATTEMPTS = 50; // Limit number of attempts

    do {
        randomFactor = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        // Scale the target position range dynamically based on zoom
        newTarget.x = (TARGET_MIN_X + randomFactor * (TARGET_MAX_X - TARGET_MIN_X)) / zoomFactor;
        newTarget.y = (static_cast<float>((rand() % 800) - 400)) / zoomFactor;

        attempts++;
        if (attempts >= MAX_ATTEMPTS) {
            // If we can't find a valid position after MAX_ATTEMPTS tries,
            // just return the last generated position
            break;
        }
    } while (isTooClose(newTarget.x, newTarget.y));

    return newTarget;
}

void Enemy::KillEnemy() {
    ++enemiesDestroyed;
    In_Game = false;
    // play enemy destroy sound.
}



/*!************************************************************************
\brief
    Default enemy movement behavior.

\param enemy
    Reference to the enemy being updated.

\param dt
    Delta time (time elapsed since the last frame).

\details
    Moves the enemy toward its target position. Once the enemy reaches
    its destination, it switches to a hovering state.
**************************************************************************/
void defaultMoveBehavior(Enemy& enemy, float dt) {
    if (!enemy.hovering) {
        AEVec2 direction;
        AEVec2Sub(&direction, &enemy.targetPosition, &enemy.Position);
        float distanceToTarget = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distanceToTarget > POSITION_THRESHOLD) {
            AEVec2Normalize(&direction, &direction);
            enemy.Position.x += direction.x * ENEMY_SPEED * dt;
            enemy.Position.y += direction.y * ENEMY_SPEED * dt;
        }
        else {
            // If the enemy reaches the target, switch to hovering.
            enemy.hovering = true;
            enemy.hoverTime = 0.0f;
        }
    }
}

/*!************************************************************************
\brief
    A faster movement behavior for enemies.

\param enemy
    Reference to the enemy executing this behavior.

\param dt
    Delta time (time elapsed since the last frame).

\details
    This function moves the enemy toward its target at an accelerated speed.
    It calculates the distance the enemy would travel in the current frame based
    on a speed multiplier (in this case, 12 times the base enemy speed) and compares
    it to the remaining distance to the target. If the calculated movement distance
    is greater than or equal to the remaining distance, the enemy's position is
    snapped to the target, and it switches to a hovering state to prevent overshooting
    and oscillation. Otherwise, the enemy's position is updated by moving it along
    the normalized direction vector multiplied by the computed movement distance.

    This mechanism ensures that fast-moving enemies do not vibrate or oscillate
    as they approach their target.
**************************************************************************/
void fastMoveBehavior(Enemy& enemy, float dt) {
    if (!enemy.hovering) {
        AEVec2 direction;
        AEVec2Sub(&direction, &enemy.targetPosition, &enemy.Position);
        float distanceToTarget = sqrt(direction.x * direction.x + direction.y * direction.y);

        // Normalize the direction vector
        AEVec2Normalize(&direction, &direction);

        // Calculate the distance the enemy would travel this frame at fast speed.
        float movementDistance = ENEMY_SPEED * 12.0f * dt;

        // If the movement distance would overshoot the target, snap to the target.
        if (movementDistance >= distanceToTarget) {
            enemy.Position = enemy.targetPosition;
            enemy.hovering = true;
            enemy.hoverTime = 0.0f;
        }
        else {
            enemy.Position.x += direction.x * movementDistance;
            enemy.Position.y += direction.y * movementDistance;
        }
    }
}

/*!*************************************************************************
\brief
    A movement behavior where the enemy periodically dodges and picks a new target.

\param enemy
    Reference to the enemy executing this behavior.

\param dt
    Delta time (time elapsed since the last frame).

\details
    This enemy will move toward a randomly chosen target. Once it reaches
    that position or after a set interval, it picks a new target position.
***************************************************************************/
void dodgeMoveBehavior(Enemy& enemy, float dt) {
    // Calculate the distance to the current target.
    AEVec2 direction;
    AEVec2Sub(&direction, &enemy.targetPosition, &enemy.Position);
    float distanceToTarget = sqrt(direction.x * direction.x + direction.y * direction.y);

    // If the enemy has reached its target, begin waiting.
    if (distanceToTarget <= POSITION_THRESHOLD) {
        // Decrease the wait timer.
        enemy.dodgeTimer -= dt;

        // Only pick a new target once the wait timer has expired.
        if (enemy.dodgeTimer <= 0.0f) {
            enemy.targetPosition = GenerateTargetPosition();
            // Reset the timer to a new wait period (2-5 seconds) before moving again.
            enemy.dodgeTimer = static_cast<float>((rand() % 3) + 2);
        }
        // Enemy stays at its target position while waiting.
    }
    // If the enemy is not yet at its target, move toward it.
    else {
        AEVec2Normalize(&direction, &direction);
        enemy.Position.x += direction.x * ENEMY_SPEED * dt;
        enemy.Position.y += direction.y * ENEMY_SPEED * dt;
    }
}

/*!************************************************************************
\brief
    Default enemy shooting behavior.

\param enemy
    Reference to the enemy executing the attack.

\param dt
    Delta time (time elapsed since the last frame).

\param EarthPosition
    Position of the Earth (enemy target).

\details
    Fires projectiles toward Earth at predefined time intervals.
**************************************************************************/
void defaultShootBehavior(Enemy& enemy, float dt, const AEVec2& targetPosition) {
    enemy.ShootTimer += dt;
    if (enemy.ShootTimer >= SHOOT_INTERVAL) {
        EnemyShoot(enemy.Position.x, enemy.Position.y, targetPosition.x, targetPosition.y);
        enemy.ShootTimer = 0.0f;
    }
}

void spreadShotBehavior(Enemy& enemy, float dt, const AEVec2& targetPosition) {
    enemy.ShootTimer += dt;
    if (enemy.ShootTimer >= SHOOT_INTERVAL) {
        int numBullets = 8; // Keep it odd so there's a center bullet
        float spreadAngle = 0.05f; // Smaller value = tighter spread

        int halfSpread = numBullets / 2; // Half go left, half go right

        float baseAngle = atan2(targetPosition.y - enemy.Position.y, targetPosition.x - enemy.Position.x);

        for (int i = -halfSpread; i <= halfSpread; i++) {
            float angle = baseAngle + (i * spreadAngle);
            float velX = cos(angle) * BULLET_SPEED;
            float velY = sin(angle) * BULLET_SPEED;
            BulletCreate(enemy.Position.x, enemy.Position.y - OFFSET_Y, velX, velY);
        }

        enemy.ShootTimer = 0.0f;
    }
}

/*!************************************************************************
\brief
    Updates the enemy's hovering movement.

\param enemy
    Reference to the enemy being updated.

\param dt
    Delta time (time elapsed since the last frame).

\details
    Enemies enter a hovering state once they reach their destination.
    This function calculates sinusoidal drift to create a floating effect.
**************************************************************************/
static void updateEnemyHovering(Enemy& enemy, float dt) {
    if (enemy.hovering) {
        enemy.hoverTime += dt;
        float angle = enemy.hoverTime * DRIFT_FREQUENCY + enemy.hoverPhase;
        float driftTargetX = enemy.targetPosition.x + DRIFT_RADIUS * cos(angle);
        float driftTargetY = enemy.targetPosition.y + DRIFT_RADIUS * sin(angle);
        enemy.Position.x = enemy.Position.x * (1 - LERP_ALPHA) + driftTargetX * LERP_ALPHA;
        enemy.Position.y = enemy.Position.y * (1 - LERP_ALPHA) + driftTargetY * LERP_ALPHA;
    }
}

/*!************************************************************************
\brief
    Handles enemy movement, shooting, and interactions with Earth.

\param dt
    Delta time (time elapsed since the last frame).

\param EarthPosition
    Position of the Earth (enemy target).

\param EarthHealth
    Reference to Earth's health, modified if an enemy collides.

\details
    Iterates through all active enemies and updates their movement,
    hovering, and attack behavior. Handles collision detection
    and applies damage to Earth if necessary.
**************************************************************************/
void EnemyMovement(float dt, AEVec2& targetPosition, float& targetHealth) {
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        if (!Enemies[i].In_Game)
            continue;

        // Use function pointer for movement.
        Enemies[i].moveBehavior(Enemies[i], dt);

        // Update hovering behavior.
        updateEnemyHovering(Enemies[i], dt);

        // Collision check with Earth.
        if (AETestCircleToCircle(&Enemies[i].Position, EARTH_COLLISION_RADIUS_ENEMY,
            &targetPosition, EARTH_COLLISION_RADIUS_EARTH)) {
            Enemies[i].In_Game = false;  // Deactivate enemy
            targetHealth -= EARTH_DAMAGE;
            if (targetHealth < 0.0f)
                targetHealth = 0.0f;
        }

        // Use function pointer for shooting.
        Enemies[i].shootBehavior(Enemies[i], dt, EarthPosition);
    }
}

/*!************************************************************************
\brief
    Renders all active enemies.

\details
    Applies transformation matrices for positioning and scaling,
    then draws each active enemy on the screen.
**************************************************************************/
void EnemyRender() {
    Camera* camera = CameraManager::GetInstance().GetCurrentCamera();

    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        if (!Enemies[i].In_Game)
            continue;
        // Set up for textured rendering
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Get the Enemy texture from MeshManager 
        AEGfxTexture* enemyTexture = MeshManager::GetInstance().GetTexture(Enemies[i].textureType);
        AEGfxTextureSet(enemyTexture, 0, 0);

        // Use much larger scale values
        float enemyScale = 50.0f; // Double the default for ENEMY1

        // Determine scale based on enemy texture type
        if (Enemies[i].textureType == TextureType::ENEMY2) {
            enemyScale = 70.0f; // 20% larger than base
        }
        else if (Enemies[i].textureType == TextureType::ENEMY3) {
            enemyScale = 90.0f; // 10% smaller than base
        }
        else if (Enemies[i].textureType == TextureType::ENEMY4) {
            enemyScale = 140.0f; // 40% larger than base
        }


        AEMtx33 camScale, scaleMatrix, transMatrix, transformMatrix;
        AEMtx33Scale(&scaleMatrix, enemyScale, enemyScale);
        AEMtx33Scale(&camScale, camera->cameraZoom, camera->cameraZoom);
        AEMtx33Trans(&transMatrix, Enemies[i].Position.x, Enemies[i].Position.y);
        AEMtx33Concat(&transformMatrix, &transMatrix, &scaleMatrix);
        AEMtx33Concat(&transformMatrix, &camScale, &transformMatrix);

        AEGfxSetTransform(transformMatrix.m);
        AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

        // Reset render mode for other elements
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    }
}

/*!************************************************************************
\brief
    Fires a burst of bullets toward Earth.

\param originX
    X-coordinate of the enemy shooting.

\param originY
    Y-coordinate of the enemy shooting.

\param targetX
    X-coordinate of the target.

\param targetY
    Y-coordinate of the target.

\details
    Spawns multiple bullets in a burst pattern aimed at Earth.
**************************************************************************/
void EnemyShoot(float originX, float originY, float targetX, float targetY) {
    // Compute normalized direction vector from enemy to target.
    AEVec2 baseDir;
    AEVec2Set(&baseDir, targetX - originX, targetY - originY);
    AEVec2Normalize(&baseDir, &baseDir);
    float baseAngle = atan2(baseDir.y, baseDir.x);
    float velX = cos(baseAngle) * BULLET_SPEED;
    float velY = sin(baseAngle) * BULLET_SPEED;
    for (int i = 0; i < BULLET_BURST_COUNT; i++) {
        float offset = BULLET_SPACING * i;
        float spawnX = originX - baseDir.x * offset;
        float spawnY = originY - baseDir.y * offset;
        BulletCreate(spawnX + OFFSET_X, spawnY - OFFSET_Y, velX, velY);
    }
}

/*!************************************************************************
\brief
    Resets all enemies when restarting the level.

\details
    Iterates through the global enemy list and deactivates all enemies.
    Resets their shooting timers and moves them off-screen, ensuring
    a fresh state when the game restarts.
**************************************************************************/
void ResetEnemies() {
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        // Ensures all values are initialized
        Enemies[i] = Enemy();
        Enemies[i].In_Game = false;
        Enemies[i].ShootTimer = 0.0f;
        Enemies[i].Position = { OFF_SCREEN_X, 0.0f };
    }
}

// LEGACY FUNCTION, FALLBACK AS ABSOLUTE LAST RESORT. 
// Logic moved to EnemyFactory::EnemyCreate()

//void EnemyCreate() {
//    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
//        if (!Enemies[i].In_Game) {
//            // Set starting position off-screen.
//            Enemies[i].Position.x = OFF_SCREEN_X;
//            Enemies[i].Position.y = static_cast<float>((rand() % 600) - 300);
//
//            // Mark enemy active and randomize shoot timer between 0.0f and 2.0f.
//            Enemies[i].In_Game = true;
//            Enemies[i].ShootTimer = static_cast<float>(rand() % 2000) / 1000.0f;
//
//            float targetX, targetY, randomFactor;
//            // Re-roll target until it is not too close to existing enemy targets.
//            do {
//                randomFactor = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//                targetX = TARGET_MIN_X + randomFactor * (TARGET_MAX_X - TARGET_MIN_X);
//                targetY = static_cast<float>((rand() % 800) - 400); // Range: [-400, 400)
//            } while (isTooClose(targetX, targetY));
//
//            Enemies[i].targetPosition.x = targetX;
//            Enemies[i].targetPosition.y = targetY;
//
//            // Initialize hovering variables.
//            Enemies[i].hovering = false;
//            Enemies[i].hoverTime = 0.0f;
//            Enemies[i].hoverPhase = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 6.28318530718f; // 0 to 2π
//
//            // Assign default behavior function pointers.
//            Enemies[i].moveBehavior = defaultMoveBehavior;
//            Enemies[i].shootBehavior = defaultShootBehavior;
//
//            break; // Only spawn one enemy per call.
//        }
//    }
//}
