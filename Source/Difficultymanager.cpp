/******************************************************************************/
/**
 * @file        DifficultyManager.hpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       Implements difficulty progression in the game
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/
#include "../Header/GameWin.hpp"
#include "../Header/HUD.hpp"
#include "../Header/DifficultyManager.hpp"
#include "AEEngine.h"

#include <iostream>
#include <vector>
#include <stdexcept>

// Global level data
LevelDatas* levelss = NULL;
int levelCounts = 0;
static float BASE_ENEMYSpawnTimer = 0.0f;
static float BASE_ENEMY_DODGESpawnTimer = 0.0f;
static float SPREAD_SHOOTER_DEFAULTSpawnTimer = 0.0f;
extern int currentLevel;
extern int counter;

// Retrieve enemy config
std::vector<EnemyConfig> const& enemyConfigs = GetEnemyConfigs();

// Enum for enemy types. The order MUST match the enemy config order.
enum EnemyTypes {
    BASE_ENEMY,                     // Corresponds to "base_enemy"
    BASE_ENEMY_FAST,                // Corresponds to "base_enemy_fast"
    BASE_ENEMY_DODGE,               // Corresponds to "base_enemy_dodge"
    SPREAD_SHOOTER_DEFAULT,         // Corresponds to "spread_shooter_default"
    SPREAD_SHOOTER_DODGE            // Corresponds to "spread_shooter_dodge"
};

// Initialize levels with enemy configurations
void InitDifficultyManager()
{
    //if (enemyConfigs.empty()) {
    //    throw std::runtime_error("Error: enemyConfigs is empty!");
    //}

    // Allocate memory for levels using AE_Allocate (Alpha Engine convention)
    levelss = (LevelDatas*)malloc(MAX_LEVELSS * sizeof(LevelDatas));
    if (!levelss) {
        //std::cerr << "Memory allocation failed!" << std::endl;
        exit(1);
    }
    for (int i = 0; i < MAX_LEVELSS; ++i) {
        levelss[i] = LevelDatas();  // Use the constructor to initialize fields
    }

    // Level configurations
    levelss[1].BASE_ENEMYCount = 3;
    levelss[1].BASE_ENEMYSpawnCount = 2;
    levelss[1].BASE_ENEMYSpawnInterval = 8.0f;

    levelss[2].BASE_ENEMYCount = 3;
    levelss[2].BASE_ENEMYSpawnCount = 2;
    levelss[2].BASE_ENEMYSpawnInterval = 8.0f;
    levelss[2].BASE_ENEMY_DODGECount = 3;
    levelss[2].BASE_ENEMY_DODGESpawnCount = 3;
    levelss[2].BASE_ENEMY_DODGESpawnInterval = 8.0f;

    levelss[3].BASE_ENEMYCount = 6;
    levelss[3].BASE_ENEMYSpawnCount = 3;
    levelss[3].BASE_ENEMYSpawnInterval = 8.0f;
    levelss[3].BASE_ENEMY_DODGECount = 4;
    levelss[3].BASE_ENEMY_DODGESpawnCount = 3;
    levelss[3].BASE_ENEMY_DODGESpawnInterval = 8.0f;
    levelss[3].SPREAD_SHOOTER_DEFAULTCount = 5;
    levelss[3].SPREAD_SHOOTER_DEFAULTSpawnCount = 3;
    levelss[3].SPREAD_SHOOTER_DEFAULTSpawnInterval = 25.0;

    levelCounts = 0;
}

// Free allocated memory
void FreeDifficultyManager()
{
    if (levelss)
    {
        free(levelss);
        levelss = nullptr;
    }
    levelCounts = 0;  // Reset to avoid invalid access
}

// Increase level difficulty
void IncrementLevel() {
    if (!levelss || levelCounts < 0 || levelCounts >= MAX_LEVELSS) {
        //std::cerr << "Invalid level access!" << std::endl;
        return;
    }
    if (levelCounts + 1 < MAX_LEVELSS) {
        ++levelCounts;
        //printf("Level increased to: %d\n", levelCounts); // Debugging print
        BASE_ENEMYSpawnTimer = 0.0f;
        BASE_ENEMY_DODGESpawnTimer = 0.0f;
        SPREAD_SHOOTER_DEFAULTSpawnTimer = 0.0f;
        ResetEnemies();  // Reset enemies when level increments
    }
    //else {
    //    printf("Max level reached! Current Level: %d\n", levelCounts);
    //}
}


// Get current level
int GetCurrLevel()
{
    return levelCounts;
}

// Spawn enemies based on timer
void HandleDifficultyManager()
{
    if (!levelss || levelCounts < 0 || levelCounts >= MAX_LEVELSS) {
        //std::cerr << "Invalid level access!" << std::endl;
        return;
    }
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime()); // Ensure frame time consistency
    static float lastSpawnTime = 0.0f;
    static float globalTimer = 0.0f;
    globalTimer += dt;

    // Minimum time between ANY enemy spawns (1 second)
    const float MIN_SPAWN_INTERVAL = 2.0f;

    // Check for level boundaries to avoid spawning past the current level
    if (levelCounts >= MAX_LEVELSS) return;

    //std::cout << "entering: " << levelss[levelCounts].BASE_ENEMYSpawnInterval << "spawntimer: " << BASE_ENEMYSpawnTimer << std::endl;

    // Update all timers
    BASE_ENEMYSpawnTimer += dt;
    BASE_ENEMY_DODGESpawnTimer += dt;
    SPREAD_SHOOTER_DEFAULTSpawnTimer += dt;

    //std::cout << "dt: " << dt << std::endl;

    // Only attempt to spawn if enough time has passed since last spawn
    if (globalTimer - lastSpawnTime < MIN_SPAWN_INTERVAL) {
        return;
    }   

    // Process ONE enemy type per frame - priority order
    //std::cout << "entering: " << levelss[levelCounts].BASE_ENEMYSpawnTimer << " spawntimer: " << BASE_ENEMYSpawnTimer << std::endl;

    // BASE_ENEMY spawning
    if (BASE_ENEMYSpawnTimer > levelss[levelCounts].BASE_ENEMYSpawnInterval)
    {
        /*std::cout << "spawning" << std::endl;*/
        BASE_ENEMYSpawnTimer = 0.0f; // Reset timer
        lastSpawnTime = globalTimer; // Track this spawn time
        /*std::cout << "spawning" << std::endl;*/
        //printf("Enemy count: %s\n", levelss);

        // Spawn just ONE enemy instead of a batch
        Enemy* availableEnemy = FindAvailableEnemySlot();
        if (availableEnemy != nullptr)
        {
            *availableEnemy = EnemyFactory::CreateEnemy(enemyConfigs[BASE_ENEMY]);
        }
        return; // Exit to ensure only one enemy type spawns per frame
    }
    //std::cout << "entering: " << levelss[levelCounts].BASE_ENEMY_DODGESpawnInterval << " spawntimer: " << BASE_ENEMY_DODGESpawnTimer << std::endl;

    // BASE_ENEMY_DODGE spawning (only if BASE_ENEMY didn't spawn)
    if (BASE_ENEMY_DODGESpawnTimer > levelss[levelCounts].BASE_ENEMY_DODGESpawnInterval)
    {
        BASE_ENEMY_DODGESpawnTimer = 0.0f; // Reset timer
        lastSpawnTime = globalTimer; // Track this spawn time

        // Spawn just ONE enemy instead of a batch
        Enemy* availableEnemy = FindAvailableEnemySlot();
        if (availableEnemy != nullptr)
        {
            *availableEnemy = EnemyFactory::CreateEnemy(enemyConfigs[BASE_ENEMY_DODGE]);
        }
        return; // Exit to ensure only one enemy type spawns per frame
    }
    //std::cout << "entering: " << levelss[levelCounts].SPREAD_SHOOTER_DEFAULTSpawnInterval << " spawntimer: " << SPREAD_SHOOTER_DEFAULTSpawnTimer << std::endl;

    // SPREAD_SHOOTER_DEFAULT spawning (only if previous types didn't spawn)
    if (SPREAD_SHOOTER_DEFAULTSpawnTimer > levelss[levelCounts].SPREAD_SHOOTER_DEFAULTSpawnInterval)
    {
        SPREAD_SHOOTER_DEFAULTSpawnTimer = 0.0f; // Reset timer
        lastSpawnTime = globalTimer; // Track this spawn time

        // Spawn just ONE enemy instead of a batch
        Enemy* availableEnemy = FindAvailableEnemySlot();
        if (availableEnemy != nullptr)
        {
            *availableEnemy = EnemyFactory::CreateEnemy(enemyConfigs[SPREAD_SHOOTER_DEFAULT]);
        }
    }
    //std::cout << "end\n";
}

// Spawn initial enemies for the current level
void SpawnInitialEnemies()
{
    if (!levelss || levelCounts < 0 || levelCounts >= MAX_LEVELSS) {
        //std::cerr << "Invalid level access!" << std::endl;
        return;
    }
    // Reset spawn timers with staggered values to prevent simultaneous spawning
    BASE_ENEMYSpawnTimer = 1.0f;
    BASE_ENEMY_DODGESpawnTimer = 1.5f;  // 1.5 second delay
    SPREAD_SHOOTER_DEFAULTSpawnTimer = 3.0f;  // 3 second delay

    // Limit initial enemies to prevent overwhelming the player
    int initialBaseEnemies = levelss[levelCounts].BASE_ENEMYCount;

    // If not level 0, don't spawn any initial enemies at all
    if (levelCounts > 0) {
        return; // Let HandleDifficultyManager handle spawning for higher levels
    }

    // Only spawn initial base enemies
    for (int i = 0; i < initialBaseEnemies; ++i)
    {
        Enemy* availableEnemy = FindAvailableEnemySlot();
        if (availableEnemy != nullptr)
        {
            *availableEnemy = EnemyFactory::CreateEnemy(enemyConfigs[BASE_ENEMY]);
        }
    }

}

void CheckLevelProgression() {
    int requiredEnemies = (currentLevel == 0) ? 10 : (currentLevel == 1) ? 15 : 30  ;
    int requiredCounter = (currentLevel == 0) ? 1 : 2;  // Required telescope level

    if (enemiesDestroyed >= requiredEnemies && counter == requiredCounter) {
        if (currentLevel < 2) {
            LoadNextLevel();
        }
        else {
            next = GS_GAMEWIN;
        }
    }
}
