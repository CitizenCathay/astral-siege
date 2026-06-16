/******************************************************************************/
/**
 * @file        Background.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (primary)
 * @brief       This file manages background layers and rendering with camera-based zoom control
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/


#pragma once

#include "AEEngine.h"
#include "../Header/Camera.hpp"
#include <vector>

class Background {
private:
    struct Layer {
        AEGfxTexture* Texture;
        float Opacity;
    };
    std::vector<Layer> Layers;
    AEGfxVertexList* pMesh;

public:
    Background();
    ~Background();

    void addLayer(AEGfxTexture* texture, float opacity);
    size_t getLayerCount();
    void renderLayerRange(int startIndex, int endIndex, Camera* camera); // Use Camera for zoom
    void cleanup();
};