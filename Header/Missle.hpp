/******************************************************************************/
/**
 * @file        Missle.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Defines the Missile class, including missile movement, aiming,
 *              shooting behavior, and upgrading mechanisms.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/GameObject.hpp"
#include "../Header/Bullet.hpp"
#include <vector>

class Camera;
class Missle : public GameObject {
public:
	Missle(AEVec2 pos, AEVec2 scale, float rotation, AEGfxVertexList* mesh, bool isActive);
	AEVec2 GetDirection() const;
	float GetDamage() const;
	void Update(std::vector<VectorField>& field, std::vector<GravityPoint>& gpVec);
	void Draw();
	BulletType GetMissleType() const;
	void UpgradeMissle();
	void ActivateSpreadshot();
	bool GetSpreadshot() const;
	static void Free();
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
	float timer;
	float damage;
	float speed;
	float distSquared;
	bool isSpreadshot;
	BulletType missleType;

	std::vector<Bullet> bulletList;
private:
	Missle();
};