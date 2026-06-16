/******************************************************************************/
/**
 * @file        Level.hpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       This file manages game levels, including level data and camera behavior.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#ifndef Level_HEADER
#define Level_HEADER

#include "../Header/Camera.hpp"
#include <stdlib.h>  // For malloc and free
#include <vector>

class CurrencyManager;
class Camera;
class OuterPlanetManager;
constexpr int MAX_LEVELS = 3;
// Struct to store level data
struct LevelData
{
    float cameraLevel;
};
// Class for Outerplanet objects
class Outerplanet : public GameObject {
public:
    Outerplanet(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, int textureType);
    void Draw();
};

extern LevelData* levels;
extern int levelCount;
void Cameralevel();
void ShopZoomOut(Camera* camera); // for telescope purchase zoom
float GetCameralevel();
void ResetGameState(Camera* camera);
void ResetGameLevel(Camera* camera);

#endif Level_HEADER