/******************************************************************************/
/**
 * @file        Vector.hpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       This header file defines the VectorField structure used to represent
 *              gravitational vector fields in the game. It computes directional influence
 *              based on gravity points, updating and rendering vector data dynamically.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/
 
#pragma once

#include <vector> // For std::vector<>
#include "../Header/gravitySystem.hpp"
#include "AEEngine.h"

/*!************************************************************************
\brief
  Represents a vector field influenced by gravity.

\details
  Each vector field element contains a position (x, y) and a directional
  component (dx, dy) affected by gravity sources. The structure provides
  a method to compute its direction based on the inverse-square law
  of gravitational influence.
**************************************************************************/
class Camera;

struct VectorField {
	float x, y;      // Position
	float dx, dy;    // Direction

	/*!************************************************************************
	\brief
	  Calculates the gravitational influence on the vector field.

	\param gravityPoints
	  A vector containing gravity points that affect the field.

	\details
	  Computes the directional influence of each gravity point using inverse
	  square law and updates the vector's directional components (dx, dy).
	**************************************************************************/
	void calculateField(const std::vector<GravityPoint>& gravityPoints, float offsetX, float offsetY);

	// Add to your VectorField class in Vector.hpp
	struct VectorCache {
		float offsetX;
		float offsetY;
		float dx;
		float dy;
		float angle;
		float magnitude;
		bool valid;
	};

	// Add to VectorField class
	std::vector<VectorCache> cache;
	void initializeCache(int cacheSize);
	bool findInCache(float offsetX, float offsetY, float& outDx, float& outDy, float& outAngle, float& outMagnitude);
	void addToCache(float offsetX, float offsetY, float inDx, float inDy, float angle, float magnitude);
};

/*!************************************************************************
\brief
  Initializes the vector field grid.

\param field
  A reference to the vector field container.

\param gridSize
  The number of vectors along one axis in the grid.

\details
  Sets up the vector field by evenly distributing vectors across the grid,
  defining initial positions, and resetting directional values.
**************************************************************************/
void InitializeVectorField(std::vector<VectorField>& field, int gridSize);

/*!************************************************************************
\brief
  Updates the gravitational influence on the vector field.

\param field
  A reference to the vector field container.

\param gravityPoints
  A vector of gravity points affecting the field.

\details
  Iterates through each vector field element and recalculates directional
  influence based on the provided gravity points.
**************************************************************************/
void UpdateVectorField(std::vector<VectorField>& field, const std::vector<GravityPoint>& gravityPoints);

/*!************************************************************************
\brief
  Renders the vector field on the screen.

\param field
  A reference to the vector field container.

\param arrowMesh
  A pointer to the mesh used for rendering vector arrows.

\param visualizeVectorsEnabled
  Boolean flag indicating whether visualization is active.

\details
  Draws each vector in the field with directional representation based
  on the computed gravitational influence.
**************************************************************************/
void RenderVectorField(std::vector<VectorField>& field, AEGfxVertexList* arrowMesh, bool visualizeVectorsEnabled, Camera* camera, const std::vector<GravityPoint>& gravityPoints);

/*!************************************************************************
\brief
  Retrieves the closest vector to a given position.

\param field
  A reference to the vector field container.

\param gridSize
  The number of vectors along one axis in the grid.

\param myPos
  The position for which the closest vector is required.

\return
  A reference to the closest VectorField element.

\details
  Finds the vector closest to the provided position using grid-based lookup
  for optimized access.
**************************************************************************/
const VectorField& GetClosestVector(std::vector<VectorField>& field, int gridSize, const AEVec2& myPos);