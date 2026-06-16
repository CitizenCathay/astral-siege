/******************************************************************************/
/**
 * @file        ExplosionManager.hpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       This file manages explosion effects and particles in the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include <vector>
#include "../Header/GameObject.hpp"

const int MAX_EMITTER = 30;
class ExplosionParticle : public GameObject {
public:
    AEVec2 accel;
    AEVec2 vel;
    float lifetime;

    ExplosionParticle() :
        accel{ 0.0f, 0.0f },
        vel{ 0.0f, 0.0f },
        lifetime(0.0f)
    {
        // Initialize GameObject parent class members if needed
    }
};

class ExplosionEmitter {
public:
    void InitMissleExplosion(float x, float y, int numParticles, float emitterTime);
    void OnUpdate(float dt);
    void OnRender();
    float GetEmitTime() const;
    bool IsAlive() const;
    const std::vector<ExplosionParticle>& GetExplosionList();
    float radius;
    AEVec2 pos;

    ExplosionEmitter() :
        radius(0.0f),
        pos{ 0.0f, 0.0f },
        emitTime(0.0f),
        isAlive(false)
    {
        // All member variables initialized
    }

private:
    std::vector<ExplosionParticle> explosionList;
    float emitTime;
    bool isAlive;
};

class ExplosionManager {
public:
    static ExplosionManager& GetInstance();
    void SpawnExplosion(AEVec2 position, float radius);
    void OnInit();
    void OnUpdate(float dt);
    void OnRender();
    ExplosionManager(ExplosionManager const&) = delete; // dont allow copy of Weapon manager
    void operator=(ExplosionManager const&) = delete;
private:
    std::vector<ExplosionEmitter> explosionList;
    ExplosionManager(); // dont allow constructing of Explosion manager
};