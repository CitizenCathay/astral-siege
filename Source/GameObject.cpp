/******************************************************************************/
/**
 * @file        GameObject.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Defines the base class for all game objects.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/GameObject.hpp"
#include "../Header/camera.hpp"
#include "../Header/MeshManager.hpp"

GameObject::GameObject() : pos{ 0, 0 }, scale{ 1, 1 }, rotation(0.0f), pMesh(nullptr), pTex(nullptr), isActive(false), textureType(-1)
{}



GameObject::GameObject(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, int tt) {
		this->pos = pos;
		this->scale = scale;
		this->rotation = rotation;
		this->pMesh = mesh;
		this->pTex = nullptr;
		if (tt >= 0 && tt < TextureType::TEXTURE_TOTAL) {
			this->pTex = MeshManager::GetInstance().GetTexture(static_cast<TextureType>(tt));
		}
		this->textureType = tt;
		this->isActive = isActive;
}

AEMtx33 GameObject::GetTransform() const{
	Camera* camera = CameraManager::GetInstance().GetCurrentCamera();
	AEMtx33 camScale;
	AEMtx33Scale(&camScale, camera->cameraZoom, camera->cameraZoom);

	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, AEDegToRad(rotation));

	// Create a translation matrix that translates by
	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	// T*R*S * matrix
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
	AEMtx33Concat(&transform, &camScale, &transform);
	return transform;
}

AEMtx33 GameObject::GetUITransform() const {
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, AEDegToRad(rotation));

	// Create a translation matrix that translates by
	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	// T*R*S * matrix
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
	
	AEMtx33 camTransMtx = { 0 };
	f32 oldCamX, oldCamY;
	AEGfxGetCamPosition(&oldCamX, &oldCamY);
	AEMtx33Trans(&camTransMtx, oldCamX, oldCamY);
	AEMtx33Concat(&transform, &camTransMtx, &transform);
	return transform;
}

// star

AEMtx33 GetStarTransform(float x, float y, float scaleX, float scaleY) {
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scale = { 0 };
	// AEMtx33Scale(&scale, scaleX * 1.75, scaleY * 1.75);
	AEMtx33Scale(&scale, scaleX, scaleY);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	// AEMtx33Trans(&translate, x * 1.75, y - 250);
	AEMtx33Trans(&translate, x, y);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	// T*R*S * matrix
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	return transform;
}

AEMtx33 GetTransform(float x, float y, float scaleX, float scaleY) {
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scale = { 0 };
	// AEMtx33Scale(&scale, scaleX * 1.75, scaleY * 1.75);
	AEMtx33Scale(&scale, scaleX, scaleY);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	// AEMtx33Trans(&translate, x * 1.75, y - 250);
	AEMtx33Trans(&translate, x, y);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	// T*R*S * matrix
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	return transform;
}

bool CheckCollision(const Particle& particle, const GravityPoint& planet, float planetRadius) {
	float dx = particle.x - planet.x;
	float dy = particle.y - planet.y;
	float distanceSquared = dx * dx + dy * dy;

	return distanceSquared <= (planetRadius * planetRadius); // Collision if within radius
}
