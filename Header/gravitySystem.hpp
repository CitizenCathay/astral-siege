/******************************************************************************/
/**
 * @file        Gravity.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This file handles gravity points and their rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"
#include <vector>

class Camera;
struct GravityPoint {
    float x = 0.0f;
    float y = 0.0f;
    float strength;
    float size;
};


void RenderGravityPoints(const std::vector<GravityPoint>& gravityPoints, AEGfxVertexList* gravityMesh, Camera* camera);