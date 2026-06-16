/******************************************************************************/
/**
 * @file        EnemyConfigLoader.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (primary)
 * @brief       This file loads and stores enemy configuration data from a JSON file.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include <string>
#include <vector>

/*!************************************************************************
\struct EnemyConfig
\brief
    Stores configuration data for an enemy.

\details
    This struct holds various properties that define an enemy,
    including its type, health, movement speed, shooting interval,
    mesh identifier, and assigned behaviors.

    These configurations are loaded from a JSON file and used
    by the `EnemyFactory` to spawn enemies with predefined attributes.

\see loadEnemyConfigs
**************************************************************************/
struct EnemyConfig {
    std::string type;
    int health;
    float speed;
    float shootInterval;
    std::string mesh;
    std::string moveBehavior;
    std::string shootBehavior;
    int currencyReward;  // Amount of currency this enemy drops when destroyed 

    // Default constructor that sets the enemy to the fallback default enemy
    EnemyConfig()
        : type{ "base_enemy" },
        health{ 10 },
        speed{ 80.0f },
        shootInterval{ 2.0f },
        mesh{ "enemy_base" },
        moveBehavior{ "default" },
        shootBehavior{ "default" },
        currencyReward{ 5 }
    {}
};

/*!************************************************************************
\brief
    Loads enemy configurations from a JSON file.

\param filename
    The path to the JSON file containing enemy configurations.

\return
    A vector of `EnemyConfig` structures representing different enemy types.

\details
    This function reads a JSON file that contains enemy attributes such as
    type, health, speed, shooting interval, movement behavior, and shooting
    behavior. It then converts the JSON data into a vector of `EnemyConfig`
    structures.

\see EnemyConfig
**************************************************************************/
std::vector<EnemyConfig> loadEnemyConfigs(const std::string& filename);