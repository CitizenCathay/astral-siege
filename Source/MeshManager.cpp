/******************************************************************************/
/**
 * @file        MeshManager.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary) - Create meshamanger structure (75%)
 * @author      Guok Yi Yong (Secondary) - Use mesh meshlist and texturelist to draw and add assets for outerplanets (10%)
 * @author      Choi Meng Yew (Secondary) - Use star mesh to render stars (5%)
 * @author      Chloe Lau Rey En (Secondary) - render UI assets (5%)
 * @author      Celeste Tong Jia Xuan (Secondary) - render UI assets (5%)
 * @brief       This file handles loading and management of mesh assets.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
				All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include "../Header/MeshManager.hpp"
#include "../Header/Background.hpp"
#include "../Header/UIManager.hpp"

MeshManager::MeshManager() {

}

MeshManager::~MeshManager() {

}

MeshManager& MeshManager::GetInstance() 
{
	static MeshManager mm;
	return mm; // singleton mesh manager.
}

void MeshManager::LoadMesh() 
{
	meshList[MeshType::TURRET1] = GetSquareCenter(0xFF00FF00);
	meshList[MeshType::TURRET2] = GetSquareCenter(0xFFfbf665);
	meshList[MeshType::BULLET1] = GetSquareCenter(0xFFFFFFFF);
	meshList[MeshType::BULLET2] = GetSquareCenter(0xFFFFFFFF);
	meshList[MeshType::WHITE_CIRCLE] = GetCircle(36, 0xFFFFFFFF);
	meshList[MeshType::EXPLOSION_CIRCLE] = GetExplosion(36, 0xFFFFFFFF);
	meshList[MeshType::MISSLE] = GetSquareCenter(0xFF7F8C8D);
	meshList[MeshType::MISSLEBULLET] = GetCircle(10, 0xFFF39C12);
	meshList[MeshType::ENEMYBULLET] = GetCircle(64, 0xFFFF6961);
	meshList[MeshType::SHIELDED] = GetForcefield(36, 0xFF00FFFF);
	meshList[MeshType::OUTERPLANET] = GetSquareCenter(0xFFB980FF);
	meshList[MeshType::QUAD] = GetSquareCenter(0xFFFFFFFF);
	meshList[MeshType::HEALTHBAR] = GetSquareCenter(0xFFFF0000);
}

void MeshManager::UnloadMesh() {
	for (int i = 0; i < TOTAL; ++i) {
		if (meshList[i]) {
			AEGfxMeshFree(meshList[i]);
			meshList[i] = nullptr;
		}
	}
}

void MeshManager::LoadTexture() 
{
	textureList[OPENSHOP] = AEGfxTextureLoad("Assets/shop.png");
	textureList[CLOSESHOP] = AEGfxTextureLoad("Assets/close.png");
	textureList[SELL] = AEGfxTextureLoad("Assets/sell.png");
	textureList[WPN1] = AEGfxTextureLoad("Assets/wpn1.png");
	textureList[WPN2] = AEGfxTextureLoad("Assets/wpn2.png");
	textureList[TURR1] = AEGfxTextureLoad("Assets/turr1.png");
	textureList[TURR2] = AEGfxTextureLoad("Assets/turr2.png");
	textureList[ZOOM] = AEGfxTextureLoad("Assets/zoom.png");
	textureList[SHIELD] = AEGfxTextureLoad("Assets/shield.png");
	textureList[EARTH] = AEGfxTextureLoad("Assets/earth.png");
	textureList[MOON] = AEGfxTextureLoad("Assets/moon.png");
	textureList[VENUS] = AEGfxTextureLoad("Assets/venus.png");
	textureList[SATURN] = AEGfxTextureLoad("Assets/saturn.png");
	textureList[SUN] = AEGfxTextureLoad("Assets/sun.png");
	textureList[URANUS] = AEGfxTextureLoad("Assets/uranus.png");
	textureList[ENEMY1] = AEGfxTextureLoad("Assets/enemy1.png");
	textureList[ENEMY2] = AEGfxTextureLoad("Assets/enemy2.png");
	textureList[ENEMY3] = AEGfxTextureLoad("Assets/enemy3.png");
	textureList[ENEMY4] = AEGfxTextureLoad("Assets/enemy4.png");
	textureList[BUTTON1] = AEGfxTextureLoad("Assets/grey.png");
	textureList[BUTTON2] = AEGfxTextureLoad("Assets/grey_pressed.png");
	textureList[BUTTONBACK] = AEGfxTextureLoad("Assets/return.png");
	textureList[BUTTONCANCEL] = AEGfxTextureLoad("Assets/cross.png");
	textureList[BUTTONPAUSE] = AEGfxTextureLoad("Assets/pause.png");
	textureList[TURR1PLC] = AEGfxTextureLoad("Assets/BronzeTurret.png");
	textureList[TURR2PLC] = AEGfxTextureLoad("Assets/SilverTurret.png");
	textureList[TURR1BLT] = AEGfxTextureLoad("Assets/BlueBullet.png");
	textureList[TURR2BLT] = AEGfxTextureLoad("Assets/OrangeBullet.png");
	textureList[MISSILE] = AEGfxTextureLoad("Assets/MotherShip.png");

	textureList[SHOP_HOVER] = AEGfxTextureLoad("Assets/shop_press.png");
	textureList[CLOSE_HOVER] = AEGfxTextureLoad("Assets/close_press.png");
	textureList[SELL_HOVER] = AEGfxTextureLoad("Assets/sell_press.png");
}

// problem with unloading textures
void MeshManager::UnloadTexture() const 
{
	for (int i = 0; i < TextureType::TEXTURE_TOTAL; ++i) 
	{
		if (textureList[i] != nullptr) {
			AEGfxTextureUnload(textureList[i]);
		}
	}
}


AEGfxVertexList* MeshManager::GetMesh(MeshType meshtype) const 
{
	if (meshtype < 0 || meshtype >= MeshType::TOTAL)
		return nullptr;
	return meshList[meshtype];
}

AEGfxTexture* MeshManager::GetTexture(TextureType textureType) const
{
	if (textureType < 0 || textureType >= TextureType::TEXTURE_TOTAL) return nullptr;
	return textureList[textureType];
}

AEGfxVertexList* MeshManager::GetSquareCorner(u32 color) 
{
	AEGfxMeshStart();

	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex

	AEGfxTriAdd(
		0.f, -1.f, color, 0.0f, 1.0f, // btm left
		1.f, -1.f, color, 1.0f, 1.0f, //btm right
		0.f, 0.f, color, 0.0f, 0.0f); // top left

	AEGfxTriAdd(
		1.f, -1.f, color, 1.0f, 1.0f, // btm right
		1.f, 0.f, color, 1.0f, 0.0f, // top right
		0.f, 0.f, color, 0.0f, 0.0f); // top left

	// Saving the mesh (list of triangles) in pMesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

AEGfxVertexList* MeshManager::GetSquareCenter(u32 color) 
{
	AEGfxMeshStart();

	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex

	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 1.0f,
		0.5f, -0.5f, color, 1.0f, 1.0f,
		-0.5f, 0.5f, color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, color, 1.0f, 1.0f,
		0.5f, 0.5f, color, 1.0f, 0.0f,
		-0.5f, 0.5f, color, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

AEGfxVertexList* MeshManager::GetCircle(int numTriangle, u32 color) 
{
	AEGfxMeshStart();

	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex
	float anglePerTriangle = 360.f / numTriangle; // how many triangle to form the circle
	float sliceRadian = PI * anglePerTriangle / 180;

	// rotation formula

	for (int i = 0; i < numTriangle; ++i) {
		int j = i + 1;
		AEGfxTriAdd(
			0.f, 0.f, color, 0.0f, 0.0f,
			(float)cos(sliceRadian * i), (float)sin(sliceRadian * i), color, 0.0f, 1.0f,
			(float)cos(sliceRadian * j), (float)sin(sliceRadian * j), color, 1.0f, 1.0f);
	}

	// Saving the mesh (list of triangles) in pMesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}
AEGfxVertexList* MeshManager::GetExplosion(int numTriangle, u32 color) {
	AEGfxMeshStart();

	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex
	float anglePerTriangle = 360.f / numTriangle; // how many triangle to form the circle
	float sliceRadian = PI * anglePerTriangle / 180;

	// rotation formula

	for (int i = 0; i < numTriangle; ++i) {
		int j = i + 1;
		AEGfxTriAdd(
			0.f, 0.f, color, 0.0f, 0.0f,
			(float)cosf(sliceRadian * i), (float)sinf(sliceRadian * i), static_cast<u32>(0), 0.0f, 1.0f,
			(float)cosf(sliceRadian * j), (float)sinf(sliceRadian * j), static_cast<u32>(0), 1.0f, 1.0f);
	}

	// Saving the mesh (list of triangles) in pMesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

AEGfxVertexList* MeshManager::GetForcefield(int numTriangle, u32 color)
{
	AEGfxMeshStart();

	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex
	float anglePerTriangle = 360.f / numTriangle; // how many triangle to form the circle
	float sliceRadian = PI * anglePerTriangle / 180;

	// rotation formula

	for (int i = 0; i < numTriangle; ++i) {
		int j = i + 1;
		AEGfxTriAdd(
			0.f, 0.f, static_cast<u32>(0), 0.0f, 0.0f,
			(float)cos(sliceRadian * i), (float)sin(sliceRadian * i), color, 0.0f, 1.0f,
			(float)cos(sliceRadian * j), (float)sin(sliceRadian * j), color, 1.0f, 1.0f);
	}

	// Saving the mesh (list of triangles) in pMesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

// star

AEGfxVertexList* createSquareMesh(unsigned int color) {
	// Each set represents a vertex, and with three sets
	// we form a single triangle.
	//
	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 1.0f,
		0.5f, -0.5f, color, 1.0f, 1.0f,
		-0.5f, 0.5f, color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, color, 1.0f, 1.0f,
		0.5f, 0.5f, color, 1.0f, 0.0f,
		-0.5f, 0.5f, color, 0.0f, 0.0f);

	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

AEGfxVertexList* createCircleMesh(unsigned int color) {
	// Start creating the mesh
	AEGfxMeshStart();

	// Define the center vertex of the circle
	float centerX = 0.0f;
	float centerY = 0.0f;

	// Add the triangles for the circle
	const int segments = 50; // Number of triangles in the circle
	float radius = 0.5f;     // Radius of the circle

	// Angle step for each segment
	float angleStep = (2.0f * PI) / segments;

	// Generate the triangles
	for (int i = 0; i < segments; ++i) {
		// Current and next angles
		float angle1 = i * angleStep;
		float angle2 = (i + 1) * angleStep;

		// Vertex positions
		f32 x1 = cos(angle1) * radius;
		f32 y1 = sin(angle1) * radius;
		f32 x2 = cos(angle2) * radius;
		f32 y2 = sin(angle2) * radius;

		// Texture coordinates for the vertices
		f32 u1 = (x1 / radius + 1.0f) * 0.5f;
		f32 v1 = (y1 / radius + 1.0f) * 0.5f;
		f32 u2 = (x2 / radius + 1.0f) * 0.5f;
		f32 v2 = (y2 / radius + 1.0f) * 0.5f;

		// Add the triangle
		AEGfxTriAdd(
			centerX, centerY, color, 0.5f, 0.5f, // Center of the circle
			x1, y1, color, u1, v1,              // Current segment vertex
			x2, y2, color, u2, v2               // Next segment vertex
		);
	}

	// Finalize and return the mesh
	AEGfxVertexList* pMesh = AEGfxMeshEnd();
	return pMesh;
}

AEGfxVertexList* createFourPointedStarMesh(unsigned int color) {
	AEGfxMeshStart();

	// Define radii for the inner and outer points of the star
	float innerRadius = 0.2f; // Smaller radius for inner points
	float outerRadius = 0.5f; // Larger radius for outer points

	// Define the angles for the 4 points (outer and inner alternating)
	const int numPoints = 8; // 4 outer + 4 inner
	float angles[numPoints] = {
		0.0f,                // Outer top
		M_PI / 4,            // Inner top-right
		M_PI / 2,            // Outer right
		3 * M_PI / 4,        // Inner bottom-right
		M_PI,                // Outer bottom
		5 * M_PI / 4,        // Inner bottom-left
		3 * M_PI / 2,        // Outer left
		7 * M_PI / 4         // Inner top-left
	};

	// Calculate vertices for the star and connect them
	for (int i = 0; i < numPoints; ++i) {
		// Determine radius based on outer or inner point
		float radius = (i % 2 == 0) ? outerRadius : innerRadius;

		// Current point
		float x1 = cos(angles[i]) * radius;
		float y1 = sin(angles[i]) * radius;

		// Next point (wrap around for the last point)
		float nextRadius = ((i + 1) % 2 == 0) ? outerRadius : innerRadius;
		float nextAngle = angles[(i + 1) % numPoints];
		float x2 = cos(nextAngle) * nextRadius;
		float y2 = sin(nextAngle) * nextRadius;

		// Add triangle: center -> current point -> next point
		AEGfxTriAdd(
			0.0f, 0.0f, color, 0.5f, 0.5f, // Center of the star
			x1, y1, color, 0.5f, 0.5f,    // Current point
			x2, y2, color, 0.5f, 0.5f     // Next point
		);
	}

	// Finalize and return the mesh
	return AEGfxMeshEnd();
}

AEGfxVertexList* createArrowMesh(unsigned int color) {
	AEGfxMeshStart();

	// Arrow body
	AEGfxTriAdd(
		-0.4f, 0.0f, color, 0.0f, 0.0f,
		0.4f, 0.0f, color, 1.0f, 0.0f,
		-0.4f, 0.05f, color, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.4f, 0.0f, color, 1.0f, 0.0f,
		0.4f, 0.05f, color, 1.0f, 1.0f,
		-0.4f, 0.05f, color, 0.0f, 1.0f);

	// Arrow head
	AEGfxTriAdd(
		0.4f, -0.15f, color, 0.0f, 0.0f,
		0.4f, 0.2f, color, 0.0f, 1.0f,
		0.7f, 0.025f, color, 1.0f, 0.5f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* createLineMesh(unsigned int color) {
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.0f, color, 0.0f, 0.0f,   // Start of the line
		0.5f, 0.0f, color, 0.0f, 0.0f,   // End of the line
		0.0f, 0.0f, color, 0.0f, 0.0f); // Center (optional, ignored for lines)
	return AEGfxMeshEnd();
}