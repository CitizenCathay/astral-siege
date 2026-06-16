/******************************************************************************/
/**
 * @file        Background.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (primary)
 * @brief       Implementation of the Background class, which handles parallax
 *              backgrounds and layered scrolling.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Background.hpp"
#include <iostream>

Background::Background() : pMesh(nullptr) {
    // Create a full-screen quad mesh using screen coordinates
    AEGfxMeshStart();
    AEGfxTriAdd(
        -1600.0f, -900.0f, 0xFFFFFFFF, 0.0f, 1.0f,  // Bottom left
        1600.0f, -900.0f, 0xFFFFFFFF, 1.0f, 1.0f,   // Bottom right
        -1600.0f, 900.0f, 0xFFFFFFFF, 0.0f, 0.0f    // Top left
    );
    AEGfxTriAdd(
        1600.0f, -900.0f, 0xFFFFFFFF, 1.0f, 1.0f,   // Bottom right
        1600.0f, 900.0f, 0xFFFFFFFF, 1.0f, 0.0f,    // Top right
        -1600.0f, 900.0f, 0xFFFFFFFF, 0.0f, 0.0f    // Top left
    );
    pMesh = AEGfxMeshEnd();
}

void Background::addLayer(AEGfxTexture* texture, float opacity) {
    Layers.push_back({ texture, opacity });
}

size_t Background::getLayerCount() {
    return Layers.size();
}

void Background::renderLayerRange(int startIndex, int endIndex, Camera* camera) {
    if (startIndex < 0 || endIndex >= static_cast<int>(Layers.size()) || startIndex > endIndex) return;
    if (!pMesh) {
        return;
    }

    // Set render states
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    // Apply zoom transformation
    AEMtx33 transform = { 0 };
    float zoom = camera->cameraZoom;  // Use the camera's zoom level
    AEMtx33Scale(&transform, zoom, zoom); // Apply zoom scaling
    AEGfxSetTransform(transform.m);

    for (int i = startIndex; i <= endIndex; i++) {
        if (!Layers[i].Texture) {
            continue;
        }

        // Set texture
        AEGfxTextureSet(Layers[i].Texture, 0, 0);
        AEGfxSetTransparency(Layers[i].Opacity);

        // Draw the mesh
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }
}


void Background::cleanup() {
    if (pMesh) {
        AEGfxMeshFree(pMesh);
        pMesh = nullptr;
    }

    for (auto& layer : Layers) {
        if (layer.Texture) {
            AEGfxTextureUnload(layer.Texture);
            layer.Texture = nullptr;
        }
    }
    Layers.clear();
}

Background::~Background() {
    cleanup();
}

