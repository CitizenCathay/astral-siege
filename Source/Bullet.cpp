/******************************************************************************/
/**
 * @file        Bullet.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Implementation of the Bullet class, which handles weapon bullets.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Bullet.hpp"
#include "../Header/Vector.hpp"
#include "../Header/Camera.hpp"
#include "../Header/Enemy.hpp"
#include "../Header/CurrencyManager.hpp"
#include "../Header/ExplosionManager.hpp"
#include "../Header/AudioManager.hpp"
#include "../Header/gamestatemanager.hpp"
#include "../Header/AudioPlayer.hpp"
#include "../Header/MeshManager.hpp"

Bullet::Bullet(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, BulletType bt)
	: GameObject(pos, scale, rotation, mesh, isActive),
	type(bt),
	direction({ 0.0f, 0.0f }), // Initialize direction to zero vector
	speed(0.0f),             // Initialize speed to 0.0f
	damage(0.0f),            // Initialize damage to 0.0f
	timer(0.0f)         // Initialize timer to 0.0f 
{
	if (bt == BulletType::BULLET1_BT || bt == BulletType::Missile_BT) {
		pTex = MeshManager::GetInstance().GetTexture(TextureType::TURR1BLT);
	}
	else if (bt == BulletType::BULLET2_BT || bt == BulletType::Missile_BT2) {
		pTex = MeshManager::GetInstance().GetTexture(TextureType::TURR2BLT);
	}
	
}

void Bullet::ActivateBullet(AEVec2 _pos, AEVec2 _direction, float _speed, float _damage) {
	timer = 0.f;
	isActive = true;
	direction = _direction;
	speed = _speed;
	damage = _damage;
	pos = _pos;
}

AEVec2 Bullet::GetDirection() const {
	return this->direction;
}

float Bullet::GetSpeed() const {
	return this->speed;
}

float Bullet::GetDamage() const {
	return this->damage;
}

float Bullet::GetAliveTime() {
	switch (type) {
	case BULLET1_BT:
		return 5.f;
	case Missile_BT:
		return 10.f;
	default:
		return 5.f;
	}
}

void Bullet::SetBulletType(BulletType newType) {
	type = newType;
	if(type == BulletType::Missile_BT2) {
		pTex = MeshManager::GetInstance().GetTexture(TextureType::TURR2BLT);
	}
}

void Bullet::Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec) {
	timer += (float)AEFrameRateControllerGetFrameTime();
	// bullet alive for 5 seconds
	if (timer > GetAliveTime()) {
		this->isActive = false;
	}
	else {
		const VectorField& vf = GetClosestVector(field, 20, pos); 
		AEVec2 closestVec2{ vf.dx, vf.dy };
		float gravityForce = 10.f; 
		AEVec2Normalize(&closestVec2, &closestVec2);
		AEVec2Scale(&closestVec2, &closestVec2, gravityForce * (float)AEFrameRateControllerGetFrameTime());
		AEVec2Scale(&direction, &direction, speed * (float)AEFrameRateControllerGetFrameTime()); // acceleration
		AEVec2Add(&direction, &direction, &closestVec2);
		AEVec2Normalize(&direction, &direction);

		AEVec2 offset;
		AEVec2Scale(&offset, &direction, speed * (float)AEFrameRateControllerGetFrameTime());
		AEVec2Add(&pos, &pos, &offset);
		rotation = atan2f(direction.y, direction.x) * 180 / PI;

		// collision with gravity and enemy
		Enemy* eList = GetEnemyList();
		for (int i = 0; i < MAX_ENEMY; ++i) {
			if (!eList[i].In_Game) {
				continue;
			}
			if (AETestCircleToCircle(&eList[i].Position, 10, &pos, scale.x)) {
				
				this->isActive = false; // Deactivate bullet upon collision
				eList[i].KillEnemy();  // Enemy dead
				if (type == BulletType::Missile_BT2) {
					ExplosionManager::GetInstance().SpawnExplosion(pos, 100.f);
				}
				else if (type == BulletType::BULLET2_BT) {
					ExplosionManager::GetInstance().SpawnExplosion(pos, 40.f);
				}

				CurrencyManager::GetInstance().IncrementCurrency(eList[i].currencyReward);
				CurrencyManager::GetInstance().ShowCollisionText(eList[i].currencyReward);

				AudioPlayer::GetInstance().PlayEnemyDestroyed();
			}
		}
		AEVec2 gPos = AEVec2();
		for (const GravityPoint& gp : gpVec) {
			gPos.x = gp.x;
			gPos.y = gp.y;
			if (AETestCircleToCircle(&gPos, gp.size / 2.f, &pos, scale.x / 2.f)) {
				this->isActive = false;
				if (type == BulletType::Missile_BT2) {
					ExplosionManager::GetInstance().SpawnExplosion(pos, 100.f);
				}
				else if (type == BulletType::BULLET2_BT) {
					ExplosionManager::GetInstance().SpawnExplosion(pos, 40.f);
				}

			}
		}
	}
}
void Bullet::Draw() {
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
