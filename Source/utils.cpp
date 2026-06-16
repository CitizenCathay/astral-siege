/******************************************************************************/
/**
 * @file        Utils.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Provides utility functions for game logic.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include "../Header/Utils.hpp"
#include "../Header/Camera.hpp"
namespace ASUtils {
	AEVec2 GetMousePositionInGame() {
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);

		CameraManager& cm = CameraManager::GetInstance();
		Camera* c = cm.GetCurrentCamera();
		f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth()) * 0.5f;
		f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;
		AEVec2 spawnPos{ x + -screenWidth,-y + screenHeight }; // formula to convert cursor position to screen position. Need add camera position x and y too.
		AEVec2Scale(&spawnPos, &spawnPos, 1 / c->cameraZoom);
		return spawnPos;
	}

	AEVec2 GetPositionInGame(s32 screenX, s32 screenY) {
		CameraManager& cm = CameraManager::GetInstance();
		Camera* c = cm.GetCurrentCamera();
		f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth()) * 0.5f;
		f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;
		AEVec2 spawnPos{ screenX + -screenWidth,-screenY + screenHeight }; // formula to convert cursor position to screen position. Need add camera position x and y too.
		AEVec2Scale(&spawnPos, &spawnPos, 1 / c->cameraZoom);
		return spawnPos;
	}

	AEVec2 GetMousePositionInUI() {
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		f32 screenWidth = static_cast<f32>(AEGfxGetWindowWidth()) * 0.5f;
		f32 screenHeight = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;
		AEVec2 spawnPos{ x + -screenWidth,-y + screenHeight }; // formula to convert cursor position to screen position. Need add camera position x and y too.
		/*AEVec2Scale(&spawnPos, &spawnPos, 1 / c->cameraZoom);*/
		return spawnPos;
	}
	AEVec2 GetMousePositionInLocal() {
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		return AEVec2{(f32)x,(f32)y};
	}
}