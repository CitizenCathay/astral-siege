/******************************************************************************/
/**
 * @file        Missle.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Defines missile objects and their behaviors.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Missle.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Utils.hpp"
#include "../Header/Camera.hpp"
#include "../Header/WeaponManager.hpp"
#include "../Header/AudioPlayer.hpp"

#include <iostream>

Missle::Missle(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive) : GameObject(pos, scale, rotation, mesh, isActive), speed(0.0f) {
	hasClicked = false;
	showIndicator = 0.f;
	AEVec2Zero(&direction); // normalize direction vector
	minSpeed = 20.f;
	maxSpeed = 1000.f;
	minDistance = 6000.f;
	maxDistance = 100000.f;
	damage = 5.f;
	distSquared = 0.f;
	timer = 0.f;
	dragDistance = 0.f;
	cooldown = 3.f;
	missleType = BulletType::Missile_BT;
	isSpreadshot = false;
	MeshManager& meshManager = MeshManager::GetInstance();
	pTex = meshManager.GetTexture(TextureType::MISSILE);
	for (int i = 0; i < 20; ++i) {
		bulletList.push_back(Bullet(AEVec2{ 0,0 }, AEVec2{ 20,20 }, 0, meshManager.GetMesh(MeshType::MISSLEBULLET), false, missleType));
	}
}

BulletType Missle::GetMissleType() const {
	return missleType;
}

void Missle::UpgradeMissle() {
	if (missleType == BulletType::Missile_BT) {
		for (int i = 0; i < 20; ++i) {
			bulletList[i].SetBulletType(BulletType::Missile_BT2);
		}
		missleType = BulletType::Missile_BT2;
	}
}

void Missle::ActivateSpreadshot() {
	isSpreadshot = true;
}

bool Missle::GetSpreadshot() const {
	return isSpreadshot;
}

AEVec2 Missle::GetDirection() const {
	return this->direction;
}

float Missle::GetDamage() const {
	return this->damage;
}

void Missle::Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec) 
{
	WeaponManager& wm = WeaponManager::GetInstance();

	if (!hasClicked && AEInputCheckCurr(VK_LBUTTON) && !wm.GetAdjustingWeapon()) {
		AEVec2 mousePos = ASUtils::GetMousePositionInGame();
		float radius = static_cast<float> (scale.y * 1.2);
		radius *= radius;
		distSquared = AEVec2SquareDistance(&pos, &mousePos);
		if (distSquared <= radius) {
			hasClicked = true;
		}
	}
	if (hasClicked && AEInputCheckReleased(VK_LBUTTON)) {
		if (showIndicator && hasClicked) {
			// Shoot bullet
			AEVec2 directions[3]{};
			directions[0] = direction;
			int numBullets = 1;
			if (isSpreadshot) {
				numBullets = 3;
				directions[1] = {direction.x, direction.y};
				directions[2] = {direction.x, direction.y};
				directions[1].x = directions[1].x + cosf(135.0f * PI / 180.0f);
				directions[1].y = directions[1].y + sinf(135.0f * PI / 180.0f);
				directions[2].x = directions[2].x + cosf(-135.0f * PI / 180.0f);
				directions[2].y = directions[2].y + sinf(-135.0f * PI / 180.0f);
			}

			AudioPlayer::GetInstance().PlayWeaponShoot();

			int shots = 0;
			for (Bullet& b : bulletList) {
				if (!b.isActive) {
					AEVec2 offsetPos;
					AEVec2Scale(&offsetPos, &direction, scale.x * 0.5f);
					AEVec2Add(&offsetPos, &pos , &offsetPos);
					b.ActivateBullet(offsetPos, directions[shots++], speed, damage);
					timer = 0.f;
					if (shots == numBullets) {
						break;
					}
				}
			}
		}
		hasClicked = false;
		showIndicator = false;
		wm.SetAdjustingWeapon(false);
	}

	if (hasClicked) {
		AEVec2 mousePos = ASUtils::GetMousePositionInGame();
		distSquared = AEVec2SquareDistance(&pos, &mousePos);
		if (distSquared >= minDistance) {
			distSquared = min(distSquared, maxDistance);
			float speedPercentage = distSquared / maxDistance;
			speed = minSpeed + speedPercentage * (maxSpeed - minSpeed);
			showIndicator = true;
			AEVec2Sub(&direction, &pos, &mousePos);
			AEVec2Normalize(&direction, &direction);
			rotation = atan2f(direction.y, direction.x) * 180 / PI;

			std::vector<GameObject>& aimIndicator = wm.GetAimIndicator();
			for (int i = 0; i < aimIndicator.size(); ++i) {
				GameObject& ind = aimIndicator[i];
				// set the position of the indicator based on drag distance.
				float halfTurrentLength = scale.x * 0.5f + 14.f;
				AEVec2 offset;
				Camera* camera = CameraManager::GetInstance().GetCurrentCamera();
				AEVec2Scale(&offset, &direction, halfTurrentLength + i * (8.f + speed / 100.f) * camera->cameraZoom);
				AEVec2Add(&ind.pos, &pos, &offset);
			}
		}
		else {
			showIndicator = false;
		}

	}

	for (Bullet& b : bulletList) {
		if (b.isActive) {
			b.Update(field, gpVec);
		}
	}
}

void Missle::Draw() {

	if (!pMesh) {
		pMesh = MeshManager::GetInstance().GetMesh(MeshType::MISSLE);
		return;
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	if (showIndicator) {
		WeaponManager& wm = WeaponManager::GetInstance();
		std::vector<GameObject>& aimIndicator = wm.GetAimIndicator();

		for (int i = 0; i < aimIndicator.size(); ++i) {
			const GameObject& ind = aimIndicator[i];
			if (!ind.pMesh) continue;

			f32 opacity = (aimIndicator.size() - i) / static_cast<float>(aimIndicator.size());
			if (distSquared >= maxDistance) {
				AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, opacity);
			}
			else {
				AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, opacity);
			}
			AEGfxSetTransform(ind.GetTransform().m);
			AEGfxMeshDraw(ind.pMesh, AE_GFX_MDM_TRIANGLES);
		}
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	}

	for (Bullet& b : bulletList) {
		if (b.isActive) {
			b.Draw();
		}
	}

	if (pTex != nullptr) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Tell Alpha Engine to use the texture stored in pTex
		AEGfxTextureSet(pTex, 0, 0);
		AEGfxSetTransform(GetTransform().m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	else {
		AEGfxSetTransform(GetTransform().m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Missle::Free()
{
	WeaponManager& wm = WeaponManager::GetInstance();
	std::vector<GameObject>& aimIndicator = wm.GetAimIndicator();

	for (GameObject& ind : aimIndicator) {
		if (ind.pMesh) {
			// Don't manually free if it's managed by MeshManager
			ind.pMesh = nullptr;
		}
	}
}