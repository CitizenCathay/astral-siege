/******************************************************************************/
/**
 * @file        Weapon.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Defines weapon objects and their behaviors.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include "AEEngine.h"
#include "../Header/Weapon.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Utils.hpp"
#include "../Header/Camera.hpp"
#include "../Header/currencyManager.hpp"
#include "../Header/AudioPlayer.hpp"
#include "../Header/WeaponManager.hpp"

// Weapon is subclass, child.
// GameObject is superclass parent.


// parametrize constructor
Weapon::Weapon(AEVec2 pos, AEVec2 scale, float rotation, bool isActive, WeaponType wpType) : GameObject(pos, scale, rotation, nullptr, isActive),
	rateOfFire(0.f),
	damage(0.f),
	bulletSpeed(0.f),
	timer(0.f),
	hasClicked(false),
	showIndicator(false),
	direction({ 0.0f, 0.0f }),
	minSpeed(0.f),
	maxSpeed(0.f),
	minDistance(0.f),
	maxDistance(0.f),
	distSquared(0.f),
	dragDistance(0.f),
	cooldown(0.f),
	rangeRadius(0.f)
	{
	// initialize things that is exclusive to weapon.
	// 'this' is a pointer to self. self refer to the created object instance.
	/*this->pos.x = 10;*/
	this->type = wpType;
	
	// after singleton implemented, this code shouldnt throw error
	MeshManager& meshManager = MeshManager::GetInstance();
	switch (wpType) {
	case WeaponType::TURRET1_WP:
		/*bulletList.resize(20);*/
		pMesh = meshManager.GetMesh(MeshType::TURRET1);
		pTex = meshManager.GetTexture(TextureType::TURR1PLC);
		rateOfFire = 1.0f;
		//rangeRadius = 400.f;
		damage = 3.f;
		bulletSpeed = 80.f;
		timer = 0.f;
		hasClicked = false;
		showIndicator = 0.f;
		AEVec2Zero(&direction); // normalize direction vector
		direction.x = -1.f;
		minSpeed = 20.f;
		maxSpeed = 1000.f;
		minDistance = 6000.f;
		maxDistance = 100000.f;
		distSquared = 0.f;
		timer = 0.f;
		dragDistance = 0.f;
		cooldown = 3.f;

		for (int i = 0; i < 20; ++i) {
			bulletList.push_back(Bullet(AEVec2{ 0,0 }, AEVec2{ 20,20 }, 0, meshManager.GetMesh(MeshType::BULLET1), false, BulletType::BULLET1_BT));
		}
		break;
	case WeaponType::TURRET2_WP:
		/*bulletList.resize(20);*/
		pMesh = meshManager.GetMesh(MeshType::TURRET2);
		pTex = meshManager.GetTexture(TextureType::TURR2PLC);
		rateOfFire = 2.0f;
		//rangeRadius = 400.f;
		damage = 3.f;
		bulletSpeed = 80.f;
		timer = 0.f;
		hasClicked = false;
		showIndicator = 0.f;
		AEVec2Zero(&direction); // normalize direction vector
		direction.x = -1.f;
		minSpeed = 20.f;
		maxSpeed = 1000.f;
		minDistance = 6000.f;
		maxDistance = 100000.f;
		distSquared = 0.f;
		timer = 0.f;
		dragDistance = 0.f;
		cooldown = 3.f;

		for (int i = 0; i < 20; ++i) {
			bulletList.push_back(Bullet(AEVec2{ 0,0 }, AEVec2{ 20,20 }, 0, meshManager.GetMesh(MeshType::BULLET2), false, BulletType::BULLET2_BT));
		}
		break;
	default:
		break;
	}

}

void Weapon::Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec) {
	//AEVec2 mousePos = ASUtils::GetMousePositionInGame(); // acting as enemy
	timer += (float)AEFrameRateControllerGetFrameTime();
	WeaponManager& wm = WeaponManager::GetInstance();

	if (!wm.GetSelling()) {
		// directional logic only allow when non-selling mode
		if (!hasClicked && AEInputCheckCurr(VK_LBUTTON) && !wm.GetAdjustingWeapon()) {
			AEVec2 mousePos = ASUtils::GetMousePositionInGame();
			float radius = static_cast<float> (scale.y * 0.5);
			radius *= radius;
			distSquared = AEVec2SquareDistance(&pos, &mousePos);
			if (distSquared <= radius) {
				hasClicked = true;
				wm.SetAdjustingWeapon(true);
			}
		}

		if (hasClicked && AEInputCheckReleased(VK_LBUTTON)) {
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
				bulletSpeed = minSpeed + speedPercentage * (maxSpeed - minSpeed);
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
					AEVec2Scale(&offset, &direction, halfTurrentLength + i * (8.f + bulletSpeed / 100.f) * camera->cameraZoom);
					AEVec2Add(&ind.pos, &pos, &offset);
				}
			}
			else {
				showIndicator = false;
			}

		}
	}
	else {
		if (AEInputCheckTriggered(VK_LBUTTON)) {
			AEVec2 mousePos = ASUtils::GetMousePositionInGame();
			float radius = static_cast<float> (scale.y * 0.5);
			radius *= radius;
			distSquared = AEVec2SquareDistance(&pos, &mousePos);
			if (distSquared <= radius) {
				CurrencyManager& cm = CurrencyManager::GetInstance();
				int discountedSellingPrice = wm.GetTurretPrice(type) / 2;
				cm.IncrementCurrency(discountedSellingPrice);
				isActive = false;
			}
		}
	}

	if (timer > rateOfFire) {
		switch (type) {
		case WeaponType::TURRET1_WP:
			ShootBullet(direction, bulletSpeed, damage);
			break;
		case WeaponType::TURRET2_WP:
			ShootBullet(direction, bulletSpeed, damage);
			break;
		default:
			break;
		}
	}

	for (Bullet& b : bulletList) {
		if (b.isActive) {
			b.Update(field, gpVec);
		}
	}
}

void Weapon::Draw() 
{
	if (showIndicator) {
		WeaponManager& wm = WeaponManager::GetInstance();
		std::vector<GameObject>& aimIndicator = wm.GetAimIndicator();

		for (int i = 0; i < aimIndicator.size(); ++i) {
			const GameObject& ind = aimIndicator[i];
			//AEGfxSetTransparency((aimIndicator.size() - i) / static_cast<float>(aimIndicator.size()));
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
	// Draw Bullets
	for (Bullet& b : bulletList) {
		if (b.isActive) {
			b.Draw();
		}
	}

	// Set transformation
	// Tell the Alpha Engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Tell Alpha Engine to use the texture stored in pTex
	AEGfxTextureSet(pTex, 0, 0);

	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

}


void Weapon::ShootBullet(AEVec2 _direction, float _speed, float _damage) {
	// check any enemy around then shoot bullet
	for (Bullet& b : bulletList) {
		if (!b.isActive) {
			b.ActivateBullet(pos, _direction, _speed, _damage);
			AudioPlayer::GetInstance().PlayWeaponShoot();
			timer = 0.f;
			break;
		}
	}
}
