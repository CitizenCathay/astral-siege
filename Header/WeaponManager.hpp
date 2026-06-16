/******************************************************************************/
/**
 * @file        WeaponManager.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Manages the creation, updating, and rendering of weapons.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include <vector>
#include <unordered_map>
#include "../Header/Weapon.hpp"
#include "../Header/Vector.hpp"
#include "../Header/gravitySystem.hpp"
#include "../Header/Missle.hpp"
#include "../Header/Shield.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Utils.hpp"
#include "../Header/Camera.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/currencyManager.hpp"
#include "../Header/Game.hpp"


class WeaponManager {
public:
	static WeaponManager& GetInstance();
	WeaponManager(WeaponManager const&) = delete; // dont allow copy of Weapon manager
	void operator=(WeaponManager const&) = delete;  // dont allow assignment of Weapon manager
	void CreateWeapon(AEVec2 position, WeaponType wpType);
	void Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec);
	void DrawWeapons();
	void ResetWeapons();
	void ActivateShield();
	void Unload();
	void SpawnPlayer();
	Missle* GetPlayer();
	std::vector<GameObject>& GetAimIndicator();

	void SetAdjustingWeapon(bool isAdjusting);
	bool GetAdjustingWeapon() const;

	void SetSelectedTurret(WeaponType selectTurretType);
	WeaponType GetSelectedTurret() const;
	int GetTurretPrice(WeaponType wpType);

	void SetSelling(bool selling);
	bool GetSelling() const;
private:
	WeaponManager();// dont allow constructing of Weapon manager
	std::vector<Weapon> weaponList;
	std::vector<GameObject> aimIndicator;
	std::unordered_map<WeaponType, int> weaponPriceMap;
	Missle* playerMissle = nullptr;
	Shield* playerShield = nullptr;
	bool isAdjustingWeapon;
	WeaponType selectedTurret;
	bool isSelling;
};