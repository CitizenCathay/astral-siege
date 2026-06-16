/******************************************************************************/
/**
 * @file        EnemyFactory.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This file defines the factory class responsible for creating enemies
 *              based on configuration data.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "../Header/Enemy.hpp"
#include "../Header/EnemyConfigLoader.hpp"

/*!************************************************************************
\class EnemyFactory
\brief
    A factory class responsible for creating enemies based on configuration data.

\details
    The `EnemyFactory` class provides a static method to create enemy instances
    using predefined configurations from an `EnemyConfig` structure. This allows
    for the dynamic creation of enemies with different behaviors, speeds, and
    attributes, supporting the data-driven approach to enemy generation.
**************************************************************************/
class EnemyFactory {
public:

    /*!
   \brief
       Creates an enemy based on the provided configuration.

   \param config
       The enemy configuration containing attributes such as health, speed,
       shooting behavior, and movement behavior.

   \return
       An `Enemy` instance initialized with the specified properties.

   \details
       This function reads the provided `EnemyConfig`, assigns the appropriate
       stats, movement and shooting behaviors, and sets up the enemy's starting
       position before returning the created enemy.
   */
    static Enemy CreateEnemy(const EnemyConfig& config);
};
