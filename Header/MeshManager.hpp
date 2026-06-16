/******************************************************************************/
/**
 * @file        MeshManager.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Manages mesh and texture loading, unloading, and access for the game.
 *              Provides functions to create shapes and assign textures.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "AEGraphics.h"
#include <unordered_map>
#include <string>

constexpr float M_PI = 3.14159265358979f; // star

enum MeshType {
	TURRET1,
	TURRET2,
	BULLET1,
	BULLET2,
	WHITE_CIRCLE,
	EXPLOSION_CIRCLE,
	MISSLE,
	MISSLEBULLET,
	ENEMYBULLET,
	OUTERPLANET,
	SHIELDED,
	QUAD,
	HEALTHBAR,
	TOTAL
};

enum TextureType {

	OPENSHOP,
	CLOSESHOP,
	SELL,
	WPN1,
	WPN2,
	TURR1,
	TURR2,
	ZOOM,
	SHIELD,
	EARTH,
	MOON,
	VENUS,
	SUN,
	URANUS,
	SATURN,
	ENEMY1,
	ENEMY2,
	ENEMY3,
	ENEMY4,
	BUTTON1,
	BUTTON2,
	BUTTONCANCEL,
	BUTTONBACK,
	BUTTONPAUSE,
	TURR1PLC,
	TURR2PLC,
	TURR1BLT,
	TURR2BLT,
	MISSILE,

	// hover versions for buttons
	SHOP_HOVER,
	CLOSE_HOVER,
	SELL_HOVER,
	TEXTURE_TOTAL,
};

class MeshManager {
public:
	static MeshManager& GetInstance();
	MeshManager(MeshManager const&) = delete; // dont allow copy of MeshManager manager
	void operator=(MeshManager const&) = delete;  // dont allow assignment of MeshManager manager
	// make this to singleton, teach everyone
	~MeshManager();
	void LoadMesh();
	void UnloadMesh();
	void LoadTexture();// By right not suppose to load all mesh/textures, eat alot RAM memory.
	void UnloadTexture() const;
	AEGfxVertexList* GetMesh(MeshType meshtype) const;
	AEGfxTexture* GetTexture(TextureType textureType) const;

private:
	MeshManager();
	AEGfxVertexList* GetCircle(int numTriangle, u32 color);
	AEGfxVertexList* GetForcefield(int numTriangle, u32 color);
	AEGfxVertexList* GetExplosion(int numTriangle, u32 color);
	AEGfxVertexList* GetSquareCorner(u32 color);
	AEGfxVertexList* GetSquareCenter(u32 color);
	AEGfxVertexList* meshList[MeshType::TOTAL] = { nullptr };
	AEGfxTexture* textureList[TextureType::TEXTURE_TOTAL] = { nullptr };
};

// star

AEGfxVertexList* createSquareMesh(unsigned int color);

AEGfxVertexList* createCircleMesh(unsigned int color);

AEGfxVertexList* createFourPointedStarMesh(unsigned int color);

AEGfxVertexList* createArrowMesh(unsigned int color);

AEGfxVertexList* createLineMesh(unsigned int color);
