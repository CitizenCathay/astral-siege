/******************************************************************************/
/**
 * @file        MainMenuObjects.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file implements the main menu ship class and its behavior.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "AEEngine.h"

class MainMenuObjects {
public:
    static float shipX;
    static float shipY;
    static float shipSpeed;
    static float shipDirectionX;
    static float shipDirectionY;
    static float shipRotation;
    static float shipRotationOffset;

    static void Initialize();
    static void ResetShip();
    static void Update(float dt);
    static void Draw();
    static void Cleanup();
};

// Add this extern declaration to the header file
extern AEGfxVertexList* shipMesh;