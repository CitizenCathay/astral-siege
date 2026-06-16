/******************************************************************************/
/**
 * @file        EnemyConfigLoader.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Loads enemy configurations from a JSON file.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/EnemyConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include "../Header/json.hpp"

using json = nlohmann::json;
/*!************************************************************************
\brief
    Loads enemy configurations from a JSON file.

\param filename
    The path to the JSON file containing enemy configurations.

\return
    A vector of `EnemyConfig` structures, each representing a different enemy type.

\details
    This function reads a JSON file containing enemy configurations. If the file
    cannot be opened, an error message is printed, and an empty vector is returned.

    The JSON file is expected to have an "enemies" array where each entry contains
    attributes like type, health, speed, shoot interval, mesh, movement behavior,
    and shooting behavior.

\see EnemyConfig
\see EnemyFactory
**************************************************************************/
std::vector<EnemyConfig> loadEnemyConfigs(const std::string& filename) {
    std::vector<EnemyConfig> configs;
    std::ifstream file{ filename };
    if (!file.is_open()) {
        //std::cerr << "Could not open JSON file: " << filename << std::endl;
        return configs;
    }

    json j;
    file >> j;

    for (const auto& enemy : j["enemies"]) {
        EnemyConfig config;
        config.type = enemy["type"].get<std::string>();
        config.health = enemy["health"].get<int>();
        config.speed = enemy["speed"].get<float>();
        config.shootInterval = enemy["shootInterval"].get<float>();
        config.mesh = enemy["mesh"].get<std::string>();
        config.moveBehavior = enemy["moveBehavior"].get<std::string>();
        config.shootBehavior = enemy["shootBehavior"].get<std::string>();
        config.currencyReward = enemy["currencyReward"].get<int>(); // for currency
        configs.push_back(config);
    }
    return configs;
}