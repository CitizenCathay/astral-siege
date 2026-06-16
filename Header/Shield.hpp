/******************************************************************************/
/**
 * @file        Shield.hpp
 * @project     Astral Siege
 * @author      Javier Lee (primary)
 * @brief       This file manages the creation, update, and rendering of the shield.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
				All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/GameObject.hpp"
const float MAX_SHIELD_HP = 1000.f;
class Shield : public GameObject {
public:
	Shield(AEVec2 pos, AEVec2 scale, float rotation, bool isActive);
	float GetHealth() const;
	float GetMaxHealth() const;
	void ActivateShield();
	void Update();
	void Draw();

protected:
	float currHP;
	
private:
	Shield();
};