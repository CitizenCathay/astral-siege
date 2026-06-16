/******************************************************************************/
/**
 * @file        OuterPlanetManager.cpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       Manages outer planet objects and their behaviors.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include <vector>

#include "../Header/OuterPlanetManager.hpp"
#include "../Header/DifficultyManager.hpp"  // Include Difficulty Manager


int currentLevel = 0;

OuterPlanetManager& OuterPlanetManager::GetInstance() {
    static OuterPlanetManager instance;
    return instance;
}
// Update Init function to add Outerplanet objects
void OuterPlanetManager::Init(int level) {
    currentLevel = level;
    outerPlanets.clear();
    gravityPoints.clear();

    MeshManager& meshManager = MeshManager::GetInstance();

    // Level 0 (Beginner level)
    if (currentLevel == 0) {
        // 4 obstacles with varied sizes and positions, some off-screen
        outerPlanets.push_back(Outerplanet(AEVec2{ -50, 150 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ -100, -170 }, AEVec2{ 160, 160 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SATURN));

        // Corresponding gravity points with radius matching planet sizes
        gravityPoints.push_back({ -50, 150, 250.0f, 70.0f }); //URANUS
        gravityPoints.push_back({ -100, -170, 350.0f, 150.0f }); //SATURN
    }
    // Level 1 (Intermediate level)
    else if (currentLevel == 1) {
        // Modified positions to reduce overlap with enemy paths
        outerPlanets.push_back(Outerplanet(AEVec2{ 200, 150 }, AEVec2{ 160, 160 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ 380, -350 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SATURN));
        outerPlanets.push_back(Outerplanet(AEVec2{ -100, 250 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::VENUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ -200, -150 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ 50, -250 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::MOON));
        outerPlanets.push_back(Outerplanet(AEVec2{ -300, 200 }, AEVec2{ 160, 160 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SATURN));
        // Off-screen obstacles moved further out
        outerPlanets.push_back(Outerplanet(AEVec2{ -500, -400 }, AEVec2{ 100, 100 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::MOON));
        outerPlanets.push_back(Outerplanet(AEVec2{ 600, 230 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::VENUS));

        // Corresponding gravity points should match the planet positions
        gravityPoints.push_back({ 200, 150, 350.0f, 150.0f }); //VENUS
        gravityPoints.push_back({ 300, -200, 250.0f, 70.0f }); //URANUS
        gravityPoints.push_back({ -100, 250, 350.0f, 70.0f }); //VENUS
        gravityPoints.push_back({ -200, -150, 250.0f, 70.0f }); //URANUS
        gravityPoints.push_back({ 50, -250, 320.0f, 120.0f }); //NEPTUNE
        gravityPoints.push_back({ -300, 200, 350.0f, 150.0f }); //VENUS
        // Off-screen
        gravityPoints.push_back({ -500, -400, 280.0f, 90.0f }); //VENUS
        gravityPoints.push_back({ 600, 230, 320.0f, 120.0f }); //NEPTUNE
    }
    // Level 2 (Advanced level - very spaced out)
    else if (currentLevel == 2) {
        // Left side planets (far from enemy paths)
        outerPlanets.push_back(Outerplanet(AEVec2{ -540, 650 }, AEVec2{ 100, 100 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SUN));
        outerPlanets.push_back(Outerplanet(AEVec2{ -480, -280 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ -1000, 450 }, AEVec2{ 160, 160 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::VENUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ -580, 120 }, AEVec2{ 100, 100 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::MOON));
        outerPlanets.push_back(Outerplanet(AEVec2{ -800, -100 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SUN));

        // Right side planets (far from enemy paths)
        outerPlanets.push_back(Outerplanet(AEVec2{ 550, -280 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ 480, 350 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::VENUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ -350, -450 }, AEVec2{ 100, 100 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SATURN));

        // Additional planets to balance the scene but maintain spacing
        outerPlanets.push_back(Outerplanet(AEVec2{ 0, 500 }, AEVec2{ 160, 160 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SATURN));
        outerPlanets.push_back(Outerplanet(AEVec2{ -200, -450 }, AEVec2{ 80, 80 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::VENUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ 200, 200 }, AEVec2{ 130, 130 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::SUN));
        outerPlanets.push_back(Outerplanet(AEVec2{ 900, 100 }, AEVec2{ 170, 170 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::URANUS));
        outerPlanets.push_back(Outerplanet(AEVec2{ 1000, -400 }, AEVec2{ 170, 170 }, 0, meshManager.GetMesh(MeshType::OUTERPLANET), true, TextureType::MOON));

        // Corresponding gravity points (matching the planet positions)
        // Left side
        gravityPoints.push_back({ -540, 650, 280.0f, 90.0f }); //VENUS
        gravityPoints.push_back({ -480, -280, 250.0f, 70.0f }); //URANUS
        gravityPoints.push_back({ -1000, 450, 350.0f, 150.0f }); //VENUS
        gravityPoints.push_back({ -580, 120, 280.0f, 90.0f }); //VENUS
        gravityPoints.push_back({ -800, -100, 320.0f, 120.0f }); //NEPTUNE

        // Right side
        gravityPoints.push_back({ 550, -280, 320.0f, 120.0f }); //NEPTUNE
        gravityPoints.push_back({ 480, 350, 320.0f, 120.0f }); //NEPTUNE
        gravityPoints.push_back({ -350, -450, 280.0f, 90.0f }); //VENUS

        // Additional
        gravityPoints.push_back({ 0, 500, 350.0f, 150.0f }); //VENUS
        gravityPoints.push_back({ -200, -450, 250.0f, 70.0f }); //URANUS
        gravityPoints.push_back({ 200, 200, 320.0f, 120.0f }); //NEPTUNE
        gravityPoints.push_back({ 900, 100, 250.0f, 70.0f }); //NEPTUNE
        gravityPoints.push_back({ 1000, -400, 250.0f, 70.0f }); //NEPTUNE
    }
}

void OuterPlanetManager::Draw() {
    // Set up for textured rendering (only once before drawing)
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);  // Use texture mode for rendering
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);     // Set blending mode
    AEGfxSetTransparency(1.0f);              // Set full opacity

    // Draw each outer planet if it is active
    for (auto& planet : outerPlanets) {
        if (planet.isActive) {
            // Ensure the planet's texture is loaded and set up
            planet.Draw();
        }
    }
}


void OuterPlanetManager::Clear() {
    outerPlanets.clear();
    gravityPoints.clear();
}

// This getter function provides access to the gravity points managed by this instance of OuterPlanetManager.
// It returns a reference to the internal vector, allowing external code to retrieve and modify 
// the gravity points directly without creating a copy.
std::vector<GravityPoint>& OuterPlanetManager::getGravityPoints() {
    // Return a reference to the vector of gravity points stored in this instance.
    // This allows direct access to modify the original gravityPoints without making a copy.
    return this->gravityPoints;
}
