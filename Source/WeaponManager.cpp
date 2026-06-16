/******************************************************************************/
/**
 * @file        WeaponManager.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Manages creation, updates, and rendering of weapons.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/WeaponManager.hpp"
#include <iostream>

WeaponManager& WeaponManager::GetInstance() {
	static WeaponManager wm;
	return wm; // Singleton Weapon Manager.
}

WeaponManager::WeaponManager() {
	isAdjustingWeapon = false;
	isSelling = false;

	MeshManager& meshManager = MeshManager::GetInstance();
	for (int i = 0; i < 10; ++i) {
		aimIndicator.push_back(GameObject(AEVec2{ 0,0 }, AEVec2{ 8,8 }, 0, meshManager.GetMesh(MeshType::WHITE_CIRCLE), false));
	}

	// How u add items in a map
	selectedTurret = WeaponType::TURRET1_WP;
	weaponPriceMap[WeaponType::TURRET1_WP] = 20;
	weaponPriceMap[WeaponType::TURRET2_WP] = 50;
}

std::vector<GameObject>& WeaponManager::GetAimIndicator() {
	// Check if any indicator has a null mesh and fix it
	MeshManager& meshManager = MeshManager::GetInstance();
	for (auto& indicator : aimIndicator) {
		if (indicator.pMesh == nullptr) {
			indicator.pMesh = meshManager.GetMesh(MeshType::WHITE_CIRCLE);
		}
	}
	return aimIndicator;
}

// similar to a lock guard to prevent multiple weapons from being rotated
void WeaponManager::SetAdjustingWeapon(bool isAdjusting) {
	isAdjustingWeapon = isAdjusting;
}
bool WeaponManager::GetAdjustingWeapon() const {
	return isAdjustingWeapon;
}

void WeaponManager::SetSelectedTurret(WeaponType selectTurretType) {
	selectedTurret = selectTurretType;
}

WeaponType WeaponManager::GetSelectedTurret() const {
	return selectedTurret;
}

int WeaponManager::GetTurretPrice(WeaponType wpType) {
	return weaponPriceMap[wpType];
}

void WeaponManager::SetSelling(bool selling) {
	isSelling = selling;
}
bool WeaponManager::GetSelling() const {
	return isSelling;
}

void WeaponManager::CreateWeapon(AEVec2 position, WeaponType wpType) {

	switch (wpType) {
	case WeaponType::TURRET1_WP:
		weaponList.push_back(Weapon(position, AEVec2{ TURRET_SIZE, TURRET_SIZE }, PI, true, WeaponType::TURRET1_WP));
		break;
	case WeaponType::TURRET2_WP:
		weaponList.push_back(Weapon(position, AEVec2{ TURRET_SIZE, TURRET_SIZE }, PI, true, WeaponType::TURRET2_WP));
		break;
	default:
		break;
	}
}

// Ctrl + A
// Ctrl + K + F
void WeaponManager::Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec) {

	// should have capability to choose weapon type
	if (AEInputCheckReleased(VK_RBUTTON)) {
		// If inside spawn zone then create, within spawn limit.
		CurrencyManager& cm = CurrencyManager::GetInstance();
		// if selectedTurret = Turret1
		//  weaponPriceMap[Turret1]  wil;l give u the value 20.
		// currency >= 20
		if (cm.GetCurrency() >= weaponPriceMap[selectedTurret]) {
			s32 minX = static_cast<s32>(AEGfxGetWindowWidth() * 0.33f);
			s32 minY = static_cast<s32>(AEGfxGetWindowHeight() * 0.78f);
			
			s32 maxX = static_cast<s32>(AEGfxGetWindowWidth() * 0.9f);
			s32 maxY = static_cast<s32>(AEGfxGetWindowHeight() * 0.122f);

			AEVec2 minPos = ASUtils::GetPositionInGame(minX, minY); // minY is positive
			AEVec2 maxPos = ASUtils::GetPositionInGame(maxX, maxY); // maxY is negative value
			AEVec2 spawnPos = ASUtils::GetMousePositionInGame();

			// Boundary Check
			if (spawnPos.x >= minPos.x && spawnPos.x <= maxPos.x && spawnPos.y >= minPos.y && spawnPos.y <= maxPos.y) {
				// Check No collision with anything, Planet, missle, turret.
				bool isThereCollision = false;
				// check position with missles, turrets, dont allow spawning near them too
				
				if (AETestCircleToCircle(&playerMissle->pos, playerMissle->scale.x / 2.f, &spawnPos, TURRET_SIZE / 2.f)) {
					isThereCollision = true;
				}

				for (Weapon& wp : weaponList) {
					if (isThereCollision) {
						break; // short circuit.
					}
					if (wp.isActive) {
						if (AETestCircleToCircle(&wp.pos, wp.scale.x / 2.f, &spawnPos, TURRET_SIZE / 2.f)) {
							isThereCollision = true;
						}
					}
				}

				// check position with planets, dont allow spawning near planets
				AEVec2 gPos = AEVec2();
				for (const GravityPoint& gp : gpVec) {
					if (isThereCollision) {
						break; // short circuit.
					}
					gPos.x = gp.x;
					gPos.y = gp.y;
					if (AETestCircleToCircle(&gPos, gp.size / 2.f, &spawnPos, TURRET_SIZE / 2.f)) {
						isThereCollision = true;
					}
				}

				if (!isThereCollision) {
					CreateWeapon(spawnPos, selectedTurret);
					cm.DeductCurrency(weaponPriceMap[selectedTurret]);
					AudioPlayer::GetInstance().PlayPurchase();
				}
				else {
					//std::cout << "Cannot Spawn Turret here!" << std::endl;
					// X icon spawn out on the clicked positions.
				}
			}
			else{
				//std::cout << "Cannot Spawn Turret here!" << std::endl;
				// X icon spawn out on the clicked positions.
			}
		}
	}

	for (Weapon& wp : weaponList) {
		if (wp.isActive) {
			wp.Update(field, gpVec);
		}
	}

	if (playerMissle && playerMissle->isActive) {
		playerMissle->Update(field, gpVec);
	}

	if (playerShield && playerShield->isActive) {
		playerShield->Update();
	}
}

void WeaponManager::DrawWeapons() {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Loops through weapons and draws active ones
	for (Weapon& wp : weaponList) {
		if (wp.isActive) {
			wp.Draw();
		}
	}

	if (playerMissle && playerMissle->isActive) {
		playerMissle->Draw();
	}

	if (playerShield && playerShield->isActive) {
		playerShield->Draw();
	}
}

// Reset all weapons
void WeaponManager::ResetWeapons() {
	weaponList.clear();
}

void WeaponManager::ActivateShield() {
	if (playerShield) {
		playerShield->ActivateShield();
	}
}

void WeaponManager::SpawnPlayer() {
	if (playerMissle == nullptr) {
		MeshManager* meshManager = &MeshManager::GetInstance();
		playerMissle = new Missle(AEVec2{ -20, 0 }, AEVec2{ 100, 40 }, 0, meshManager->GetMesh(MeshType::MISSLE), true);
	}

	if (playerShield == nullptr) {
		playerShield = new Shield(EarthPosition, AEVec2{ 450, 450 }, 0, false);
	}
}

void WeaponManager::Unload() {
	if (playerMissle != nullptr) {
		delete playerMissle;
		playerMissle = nullptr;
	}
	if (playerShield != nullptr) {
		delete playerShield;
		playerShield = nullptr;
	}
}

Missle* WeaponManager::GetPlayer() {
	return playerMissle;
}
	
