/******************************************************************************/
/**
 * @file        Bullet.hpp
 * @project     Astral Siege
 * @author      Javier Lee (primary)
 * @brief       This file defines bullet properties and behavior, including movement,
 *              activation, and rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
				All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/GameObject.hpp"

enum BulletType {
	BULLET1_BT,
	BULLET2_BT,
	Missile_BT, // 
	Missile_BT2, // bigger explosion
	TOTALBULLETS
};

class Camera;
class Bullet : public GameObject {
public:
	Bullet(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive, BulletType bt);
	void ActivateBullet(AEVec2 _pos, AEVec2 _direction, float _speed, float _damage);
	AEVec2 GetDirection() const;
	float GetSpeed() const;
	float GetDamage() const;
	void Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec);
	void Draw();
	float GetAliveTime();
	void SetBulletType(BulletType newType);
protected:
	//AEVec2 position; // x, y
	AEVec2 direction; // normalize direction vector
	float speed;
	float damage;
	float timer;
	BulletType type;
private:
	Bullet();
};

extern int enemiesDestroyed;
