/******************************************************************************/
/**
 * @file        DifficultyManager.hpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       This file handles difficulty progression in the game
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#ifndef DifficultyManager_HEADER
#define DifficultyManager_HEADER
#include "AEEngine.h"
#include <vector>
#include <stdlib.h>  // For malloc and free
#include "../Header/game.hpp"

constexpr int MAX_LEVELSS = 4;

struct LevelDatas
{
    int BASE_ENEMYCount;
    int BASE_ENEMY_DODGECount;
    int SPREAD_SHOOTER_DEFAULTCount;
    int BASE_ENEMYSpawnCount;
    int BASE_ENEMY_DODGESpawnCount;
    int SPREAD_SHOOTER_DEFAULTSpawnCount;
    float BASE_ENEMYSpawnInterval;
    float BASE_ENEMY_DODGESpawnInterval;
    float SPREAD_SHOOTER_DEFAULTSpawnInterval;

    // Add timers for enemy spawns
    float BASE_ENEMYSpawnTimer;
    float BASE_ENEMY_DODGESpawnTimer;
    float SPREAD_SHOOTER_DEFAULTSpawnTimer;

    // Default constructor
    LevelDatas()
        : BASE_ENEMYCount(10),
        BASE_ENEMY_DODGECount(5),
        SPREAD_SHOOTER_DEFAULTCount(3),
        BASE_ENEMYSpawnCount(2),
        BASE_ENEMY_DODGESpawnCount(1),
        SPREAD_SHOOTER_DEFAULTSpawnCount(1),
        BASE_ENEMYSpawnInterval(1.0f),
        BASE_ENEMY_DODGESpawnInterval(1.5f),
        SPREAD_SHOOTER_DEFAULTSpawnInterval(2.0f),
        BASE_ENEMYSpawnTimer(0.0f),
        BASE_ENEMY_DODGESpawnTimer(0.0f),
        SPREAD_SHOOTER_DEFAULTSpawnTimer(0.0f)
    {}
};

extern LevelDatas* levelss;
extern int levelCounts;
// Initialize levels
void InitDifficultyManager();
//extern LevelData* levels;
// Destroy allocated memory
void FreeDifficultyManager();

// Increase level difficulty
void IncrementLevel();

// Get current level
int GetCurrLevel();

// Spawn enemies based on timer
void HandleDifficultyManager();

// Spawn initial enemies for the current level
void SpawnInitialEnemies();

// Check the level condition
void CheckLevelProgression();

#endif DifficultyManager_HEADER