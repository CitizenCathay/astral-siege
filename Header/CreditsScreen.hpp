/* Start Header ************************************************************************/
/**
 * @file        CreditsScreen.hpp
 * @brief       Manages the credits screen, including loading, updating,
 *              rendering, and cleanup of credits-related UI elements.
 * @author      Celeste Tong Jia Xuan (celestejiaxuan.tong@digipen.edu)
 * @date        03/2025
 * @copyright   Copyright (C) 2025 DigiPen Institute of Technology.
 *              Reproduction or disclosure of this file or its contents
 *              without the prior written consent of DigiPen Institute of
 *              Technology is prohibited.
 */
 /* End Header **************************************************************************/

#pragma once

#include "AEEngine.h"

void Credits_Load();
void Credits_Initialize();
void Credits_Update();
void Credits_Draw();
void Credits_Free();
void Credits_Unload();