/******************************************************************************/
/**
 * @file        ExplosionManager.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Handles the creation and update of explosion effects.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include "../Header/ExplosionManager.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Enemy.hpp"
#include "../Header/CurrencyManager.hpp"

ExplosionManager::ExplosionManager() {
    explosionList.reserve(MAX_EMITTER);
    for (int i = 0; i < MAX_EMITTER; ++i) {
        explosionList.push_back(ExplosionEmitter());
    }
    
}

ExplosionManager& ExplosionManager::GetInstance() {
    static ExplosionManager instance;
    return instance;
}

void ExplosionManager::SpawnExplosion(AEVec2 position, float radius) {
    for (ExplosionEmitter& emitter: explosionList) {
        if (emitter.IsAlive()) {
            continue;
        }
        emitter.pos = position;
        emitter.radius = radius;
        emitter.InitMissleExplosion(position.x, position.y, 100, 3.f); // Spawn 100 particles per explosion
        return;
    }
}

void ExplosionManager::OnInit() {
   
}


void ExplosionManager::OnUpdate(float dt) {
    for (auto& emitter : explosionList) {
        if (!emitter.IsAlive()) {
            continue;
        }
        emitter.OnUpdate(dt);
    }
}

void ExplosionManager::OnRender() {
    for (auto& emitter : explosionList) {
        emitter.OnRender();
    }
}

void ExplosionEmitter::InitMissleExplosion(float x, float y, int numParticles, float emitterTime) {
    isAlive = true;
    emitTime = emitterTime;
    pos.x = x;
    pos.y = y;
    explosionList.clear();

    for (int i = 0; i < numParticles; ++i) {
        ExplosionParticle particle;
       
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;
        float speed = static_cast<float>(rand()) / RAND_MAX * 30.f;
        if (i < 25) {
            particle.pos.x = pos.x + cos(angle) * radius;
            particle.pos.y = pos.y + sin(angle) * radius;
        }
        else {
            particle.pos.x = pos.x + cos(angle) * radius * AERandFloat();
            particle.pos.y = pos.y + sin(angle) * radius * AERandFloat();
        }
   
        particle.vel.x = cos(angle) * -speed;
        particle.vel.y = sin(angle) * -speed;
        particle.accel.x = 0.0f;
        particle.accel.y = -9.8f;
        particle.lifetime = 1.5f;
        float scale = static_cast<float>(rand()) / RAND_MAX * (radius/6.f) + 2.0f;
        particle.scale.x = scale;
        particle.scale.y = scale;
        explosionList.push_back(particle);
    }
}

void ExplosionEmitter::OnUpdate(float dt) {

    for (auto it = explosionList.begin(); it != explosionList.end(); ) {
        it->vel.x += it->accel.x * dt;
        it->vel.y += it->accel.y * dt;
        it->pos.x += it->vel.x * dt;
        it->pos.y += it->vel.y * dt;
        it->scale.x += dt*10;
        it->scale.y += dt*10;
        it->lifetime -= dt;

        if (it->lifetime <= 0) {
            it = explosionList.erase(it);
        }
        else {
            ++it;
        }
    }
    if (explosionList.size() == 0) {
        isAlive = false;
    }
    Enemy* eList = GetEnemyList();
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (!eList[i].In_Game) {
            continue;
        }
        if (AETestCircleToCircle(&eList[i].Position, 10, &pos, radius)) {
            eList[i].KillEnemy();  // Deactivate bullet upon collision
			CurrencyManager::GetInstance().IncrementCurrency(eList[i].currencyReward);
        }
    }
}

void ExplosionEmitter::OnRender() {
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_ADD);
    AEGfxSetColorToMultiply(0.952941f, 0.611765f, 0.070588f, 1.0f);
    MeshManager& mm = MeshManager::GetInstance();
    for (const auto& particle : explosionList) {
     
        AEGfxSetTransform(particle.GetTransform().m);
        AEGfxMeshDraw(mm.GetMesh(MeshType::EXPLOSION_CIRCLE), AE_GFX_MDM_TRIANGLES);
    }
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetColorToMultiply(1.0, 1.f, 1.f, 1.f);

}

// getter for array for number of explosions
const std::vector<ExplosionParticle>& ExplosionEmitter::GetExplosionList() {
    return explosionList;
}

// getter for particle lifetime
float ExplosionEmitter::GetEmitTime() const {
    return emitTime;
}
bool ExplosionEmitter::IsAlive() const {
    return isAlive;
}