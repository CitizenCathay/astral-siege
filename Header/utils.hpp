/******************************************************************************/
/**
 * @file        Utils.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Utility functions for handling mouse positions and input.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "AEGraphics.h"
namespace ASUtils {
	AEVec2 GetMousePositionInGame();
	AEVec2 GetPositionInGame(s32 screenX, s32 screenY);
	AEVec2 GetMousePositionInLocal();
	AEVec2 GetMousePositionInUI();
}