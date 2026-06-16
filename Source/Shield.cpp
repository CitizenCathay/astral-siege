/******************************************************************************/
/**
 * @file        Shield.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Activates the shield, manages shield health, detects collisions 
 *				with enemy bullets or ramming enemies, and handles rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Shield.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/enemybullet.hpp"
#include "../Header/Camera.hpp"
#include "../Header/Enemy.hpp"
#include "../Header/CurrencyManager.hpp"


Shield::Shield(AEVec2 pos, AEVec2 scale, float rotation, bool isActive) : GameObject(pos, scale, rotation, 
	MeshManager::GetInstance().GetMesh(MeshType::SHIELDED), isActive) {
	currHP = 0;
}

void Shield::ActivateShield() {
	currHP = MAX_SHIELD_HP;
	isActive = true;
}

float Shield::GetHealth() const {
	return currHP;
}
float Shield::GetMaxHealth() const {
	return MAX_SHIELD_HP;
}

void Shield::Update() {
    BulletEnemy* bullets = GetEnemyBullet();
	for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].In_Game) {
            // Use AETestCircleToCircle for collision detection
            if (AETestCircleToCircle(&bullets[i].Position, BULLET_SCALE, &pos, scale.x)) {
                bullets[i].In_Game = false;     // Deactivate bullet upon collision
                currHP -= 5.f;            // Decrease Earth's health
                //StartCameraShake(0.1f, 0.1f);   // Shake for 0.1 seconds with intensity 0.1
                if (currHP < 0.0f) {
                    currHP = 0.0f;  // Prevent negative health
                    isActive = false;
                }
            }
        }
	}

	Enemy* eList = GetEnemyList();
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!eList[i].In_Game) {
			continue;
		}

		// if the enemy rams into the shield
		if (AETestCircleToCircle(&eList[i].Position, 10, &pos, scale.x)) {
			this->isActive = false;
			eList[i].In_Game = false;  // Deactivate bullet upon collision
			currHP -= 5.f;
			CurrencyManager::GetInstance().IncrementCurrency(eList[i].currencyReward);
			if (currHP < 0.0f) {
				currHP = 0.0f;  // Prevent negative health
				isActive = false;
			}
			
		}
	}
}

void Shield::Draw() {
	AEGfxSetTransparency(currHP/GetMaxHealth());
    AEGfxSetTransform(GetTransform().m);
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}