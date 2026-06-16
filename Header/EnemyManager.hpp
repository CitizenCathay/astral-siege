/******************************************************************************/
/**
 * @file        EnemyManager.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This file manages enemy configurations and spawning in the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/EnemyConfigLoader.hpp"

/*!************************************************************************
\brief
    Retrieves the global enemy configuration list.

\return
    A reference to the vector containing all loaded enemy configurations.

\details
    This function provides access to the list of enemy configurations that were
    loaded from JSON. The configurations define enemy properties such as speed,
    health, behaviors, and visuals, which are used by the `EnemyFactory` to
    create enemies dynamically.

\see InitializeEnemies
**************************************************************************/
const std::vector<EnemyConfig>& GetEnemyConfigs();

/*!************************************************************************
\brief
    Finds an available enemy slot in the global enemy list.

\return
    Pointer to an available `Enemy` slot if found, `nullptr` if no slots are available.

\details
    This function iterates through the global enemy list and checks for an inactive
    enemy (i.e., one that is not currently in the game). If an available slot is found,
    a pointer to that enemy slot is returned. If no free slots exist, the function
    returns `nullptr`. This is used when spawning new enemies to ensure they do not
    overwrite existing active enemies.

\see GetEnemyConfigs
**************************************************************************/
Enemy* FindAvailableEnemySlot();

/*!************************************************************************
\brief
    Loads enemy configurations from a JSON file.

\details
    This function initializes the global enemy configuration list by reading
    predefined enemy types from an external JSON file. The loaded configurations
    are used by the enemy factory to create enemies dynamically.

\see GetEnemyConfigs
**************************************************************************/
void InitializeEnemies();