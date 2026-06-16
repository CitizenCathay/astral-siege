/******************************************************************************/
/**
 * @file        Vector.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Defines vector fields and gravitational interactions.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <iostream>
#include "AEEngine.h"
#include "../Header/Vector.hpp"
#include "../Header/Camera.hpp"

// Update the calculateField method to take offset parameters
void VectorField::calculateField(const std::vector<GravityPoint>& gravityPoints, float offsetX = 0.0f, float offsetY = 0.0f)
{
    dx = 0.0f; // Reset direction
    dy = 0.0f;

    // Calculate actual world position with offset
    float worldX = (x * 800.0f) + offsetX;
    float worldY = (y * 450.0f) + offsetY;

    for (const auto& point : gravityPoints) {
        // Calculate direction relative to actual position
        float diffX = point.x - worldX;
        float diffY = point.y - worldY;

        // Calculate squared distance
        float distSq = diffX * diffX + diffY * diffY;

        // Inverse-square law
        if (distSq > 0.01f) { // 0.1^2 = 0.01
            float scale = point.strength / distSq;
            dx += diffX * scale;
            dy += diffY * scale;
        }
    }

    // Normalize if needed
    float mag = sqrt(dx * dx + dy * dy);
    if (mag > 0.0f) {
        dx /= mag;
        dy /= mag;
    }
}

void InitializeVectorField(std::vector<VectorField>& field, int gridSize) {
    field.clear();
    float step = 2.0f / (gridSize - 1);  // Range from -1 to 1

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            VectorField vector{};
            vector.x = -1.0f + i * step;
            vector.y = -1.0f + j * step;
            vector.initializeCache(20); // Cache size - adjust as needed
            field.push_back(vector);
        }
    }
}

void UpdateVectorField(std::vector<VectorField>& field, const std::vector<GravityPoint>& gravityPoints) {
	for (auto& vector : field) {
		// Update vector direction using all gravity points
		vector.calculateField(gravityPoints);
	}
}

void RenderVectorField(std::vector<VectorField>& field, AEGfxVertexList* arrowMesh, bool visualizeVectorsEnabled, Camera* camera, const std::vector<GravityPoint>& gravityPoints) {
    // Set rendering mode and transparency
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(0.5f);

    float zoom = camera->cameraZoom;

    // Calculate visible area bounds in world space
    float visibleWidth = 800.0f / zoom;
    float visibleHeight = 450.0f / zoom;

    // Calculate bounds for repetitions (center around camera position)
    float centerX = camera->position.x;
    float centerY = camera->position.y;
    
    int minRx = static_cast<int>(floor((centerX - visibleWidth/2) / 1600.0f)) - 1;
    int maxRx = static_cast<int>(ceil((centerX + visibleWidth/2) / 1600.0f)) + 1;
    int minRy = static_cast<int>(floor((centerY - visibleHeight/2) / 900.0f)) - 1;
    int maxRy = static_cast<int>(ceil((centerY + visibleHeight/2) / 900.0f)) + 1;

    // Check for gravity points changes
    static int lastGravityHash = 0;
    int currentGravityHash = 0;
    for (const auto& point : gravityPoints) {
        // Simple hash combining position and strength
        currentGravityHash += static_cast<int>(point.x * 100 + point.y * 10 + point.strength);
    }
    bool gravityChanged = (lastGravityHash != currentGravityHash);
    lastGravityHash = currentGravityHash;

    // For each vector in our original grid
    for (auto& vector : field) {
        // Repeat the vector field in all directions within visible area
        for (int rx = minRx; rx <= maxRx; ++rx) {
            for (int ry = minRy; ry <= maxRy; ++ry) {
                // Calculate offsets for this repetition
                float offsetX = rx * 1600.0f;
                float offsetY = ry * 900.0f;

                // Calculate position with offset
                float posX = vector.x * 800.0f + offsetX;
                float posY = vector.y * 450.0f + offsetY;

                float cachedDx = 0.0f, cachedDy = 0.0f, cachedAngle = 0.0f, cachedMagnitude = 0.0f;
                bool cacheHit = !gravityChanged && vector.findInCache(offsetX, offsetY, cachedDx, cachedDy, cachedAngle, cachedMagnitude);
                
                if (!cacheHit) {
                    // Recalculate field direction for this position
                    vector.calculateField(gravityPoints, offsetX, offsetY);
                    
                    // Calculate rotation angle from direction
                    float angle = atan2(vector.dy, vector.dx);
                    
                    // Calculate magnitude for scaling
                    float magnitude = sqrt(vector.dx * vector.dx + vector.dy * vector.dy);
                    magnitude = (magnitude < 1.0f ? magnitude : 1.0f); // Clamp magnitude
                    
                    // Add to cache
                    vector.addToCache(offsetX, offsetY, vector.dx, vector.dy, angle, magnitude);
                    
                    cachedDx = vector.dx;
                    cachedDy = vector.dy;
                    cachedAngle = angle;
                    cachedMagnitude = magnitude;
                }

                // Create transform matrix with zoom factor
                AEMtx33 scale{ 0 };
                AEMtx33Scale(&scale, 50.0f * cachedMagnitude * zoom, 50.0f * zoom);
                
                AEMtx33 rotate{ 0 };
                AEMtx33Rot(&rotate, cachedAngle);
                
                AEMtx33 translate{ 0 };
                AEMtx33Trans(&translate, posX * zoom, posY * zoom);
                
                AEMtx33 transform{ 0 };
                AEMtx33Concat(&transform, &rotate, &scale);
                AEMtx33Concat(&transform, &translate, &transform);
                
                if (visualizeVectorsEnabled) {
                    // Set transform and draw
                    AEGfxSetTransform(transform.m);
                    AEGfxMeshDraw(arrowMesh, AE_GFX_MDM_TRIANGLES);
                }
            }
        }
    }
}

// We should not loop the vector field to find the closest vector.
// we should use grid base position to find the array index of the grid.
// dont have to loop through 400 grid to find closest.
const VectorField& GetClosestVector(std::vector<VectorField>& field, int gridSize, const AEVec2& myPos) {
	// myPos need to convert this to -1 to 1
	int xGridSizeScreen = AEGfxGetWindowWidth() / gridSize;
	int yGridSizeScreen = AEGfxGetWindowHeight() / gridSize;
	// field is [col][row] ->  need to make into [row][col]
	// we rotate ccw 90 degree, (-y,x)
	// then now we want y axis to point downward like how array works (--y,x)
	float x = myPos.x + AEGfxGetWindowWidth() * 0.5f;
	float y = myPos.y + AEGfxGetWindowHeight() * 0.5f;

	int xIndex = int(x) / xGridSizeScreen;
	int yIndex = int(y) / yGridSizeScreen;

	xIndex = (xIndex < 0) ? 0 : ((xIndex >= gridSize) ? gridSize - 1 : xIndex);
	yIndex = (yIndex < 0) ? 0 : ((yIndex >= gridSize) ? gridSize - 1 : yIndex);


	// dont need loop 400 times
	return field[static_cast<std::vector<VectorField, std::allocator<VectorField>>::size_type>(xIndex) * gridSize + yIndex];
	
}

void VectorField::initializeCache(int cacheSize) {
    cache.clear();
    cache.resize(cacheSize);
    for (auto& entry : cache) {
        entry.valid = false;
    }
}

bool VectorField::findInCache(float offsetX, float offsetY, float& outDx, float& outDy, float& outAngle, float& outMagnitude) {
    for (const auto& entry : cache) {
        if (entry.valid &&
            std::abs(entry.offsetX - offsetX) < 0.1f &&
            std::abs(entry.offsetY - offsetY) < 0.1f) {
            outDx = entry.dx;
            outDy = entry.dy;
            outAngle = entry.angle;
            outMagnitude = entry.magnitude;
            return true;
        }
    }
    return false;
}

void VectorField::addToCache(float offsetX, float offsetY, float inDx, float inDy, float angle, float magnitude) {
    // Find an empty spot or replace the oldest entry
    static int nextCacheIndex = 0;

    cache[nextCacheIndex].offsetX = offsetX;
    cache[nextCacheIndex].offsetY = offsetY;
    cache[nextCacheIndex].dx = inDx;
    cache[nextCacheIndex].dy = inDy;
    cache[nextCacheIndex].angle = angle;
    cache[nextCacheIndex].magnitude = magnitude;
    cache[nextCacheIndex].valid = true;

    nextCacheIndex = (static_cast<unsigned long long>(nextCacheIndex) + 1) % cache.size();
}