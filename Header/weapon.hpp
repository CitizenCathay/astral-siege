/******************************************************************************/
/**
 * @file        Weapon.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Defines the Weapon class for shooting mechanics and bullet handling.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/GameObject.hpp"
#include "../Header/Bullet.hpp"
#include "../Header/Vector.hpp"
#include "../Header/gravitySystem.hpp"
#include <vector>
enum WeaponType {
	TURRET1_WP,
	TURRET2_WP,
	TOTALWEAPONS
};

const f32 TURRET_SIZE = 40.f;

class Camera;

class Weapon: public GameObject {
public:
	Weapon(AEVec2 pos, AEVec2 scale, float rotation,
		bool isActive, WeaponType weaponType);
	void ShootBullet(AEVec2 _direction, float _speed, float _damage);
	void Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec);
	void Draw() ;
private:
	Weapon();
	std::vector<Bullet> bulletList;
	WeaponType type;
	float rateOfFire;
	float timer;
	float rangeRadius;
protected:
	bool hasClicked;
	bool showIndicator;
	AEVec2 direction; // normalize direction vector
	float minSpeed;
	float maxSpeed;
	float minDistance;
	float maxDistance;
	float dragDistance;
	float cooldown;
	float damage;
	float bulletSpeed;
	float distSquared;
};
