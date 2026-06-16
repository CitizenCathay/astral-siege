/******************************************************************************/
/**
 * @file        OuterPlanetManager.hpp
 * @project     Astral Siege
 * @author      Guok Yi Yong (Primary)
 * @brief       Manages the initialization, updating, and rendering of
 *              outer planets and gravity points.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include <vector>
#include "../Header/game.hpp"

extern AEGfxVertexList* gravityPointMesh;  // Declare the variable
extern OuterPlanetManager outerPlanetManager;

class OuterPlanetManager {
public:
    void Init(int level);
    void Draw();
    void Clear();
    // Retrieves a reference to the private data member gravityPoints.
    // Singleton instance retrieval
    static OuterPlanetManager& GetInstance();
    std::vector<GravityPoint>& getGravityPoints();
    // Private constructor for Singleton
    OuterPlanetManager() = default;

    // Prevent copying
    OuterPlanetManager(const OuterPlanetManager&) = delete;
    OuterPlanetManager& operator=(const OuterPlanetManager&) = delete;
private:
    std::vector<Outerplanet> outerPlanets;
    std::vector<GravityPoint> gravityPoints;
    std::vector<VectorField> gravityField;
};