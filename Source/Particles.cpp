/******************************************************************************/
/**
 * @file        Particles.cpp
 * @project     Astral Siege
 * @author      Choi Meng Yew (Primary)
 * @brief       Handles particle effects and rendering.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/Particles.hpp"
#include "../Header/GameObject.hpp"
#include "../Header/Camera.hpp"
#include <cstdlib>  // For rand()

bool IsInsidePlanet(float x, float y, const std::vector<GravityPoint>& gravityPoints) {
    for (const auto& planet : gravityPoints) {
        float dx = x - planet.x;
        float dy = y - planet.y;
        float distanceSquared = dx * dx + dy * dy;
        float planetRadiusSquared = (planet.size / 2.0f) * (planet.size / 2.0f);
        if (distanceSquared <= planetRadiusSquared) {
            return true; // Inside a planet
        }
    }
    return false; // Not inside any planet
}

void SpawnParticles(std::vector<Particle>& particles, int count, bool particlesEnable, const std::vector<GravityPoint>& gravityPoints) {
    if (particlesEnable) {
        for (int i = 0; i < count; ++i) {
            Particle particle{};
            do {
                // Generate random position
                particle.x = static_cast<float>(rand() % 3200 - 1600);  // Random position within screen width
                particle.y = static_cast<float>(rand() % 1800 - 900);   // Random position within screen height
            } while (IsInsidePlanet(particle.x, particle.y, gravityPoints)); // Regenerate position if inside a planet

            particle.vx = static_cast<float>(rand() % 200 - 100) / 100.0f;  // Random velocity
            particle.vy = static_cast<float>(rand() % 200 - 100) / 100.0f;
            particle.ax = 0.0f;  // Initial acceleration
            particle.ay = 0.0f;
            particle.lifetime = 8.0f;  // Lifetime in seconds
            particles.push_back(particle);
        }
    }
}


void SpawnExplosion(std::vector<Explosion>& explosions, float x, float y) {
    Explosion explosion;
    explosion.x = x;
    explosion.y = y;
    explosion.size = 20.0f;      // Initial size
    explosion.lifetime = 1.0f;   // Explosion lasts 1 second
    explosions.push_back(explosion);
}

void UpdateParticles(std::vector<Particle>& particles,
    std::vector<VectorField>& field,
    float deltaTime,
    const std::vector<GravityPoint>& gravityPoints,
    std::vector<Explosion>& explosions) {
    for (Particle& particle : particles) {
        // Find the closest vector field cell
        VectorField* closestVector = nullptr;
        float minDist = FLT_MAX;

        for (VectorField& vector : field) {
            float dx = particle.x - vector.x * 800.0f; // Map to screen space
            float dy = particle.y - vector.y * 450.0f;
            float dist = dx * dx + dy * dy; // Squared distance

            if (dist < minDist) {
                minDist = dist;
                closestVector = &vector;
            }
        }

        // Apply the vector field's force to the particle
        if (closestVector) {
            particle.ax = closestVector->dx * 10.0f; // Scale gravity effect
            particle.ay = closestVector->dy * 10.0f;
        }

        // Update velocity
        particle.vx += particle.ax * deltaTime;
        particle.vy += particle.ay * deltaTime;

        // Update position
        particle.x += particle.vx * deltaTime;
        particle.y += particle.vy * deltaTime;

        // Decrease lifetime
        particle.lifetime -= deltaTime;

        // Check collisions with all gravity points
        for (const auto& planet : gravityPoints) {
            float planetRadius = planet.size / 2.0f;
            if (CheckCollision(particle, planet, planetRadius)) {
                // Spawn an explosion if collision is detected
                SpawnExplosion(explosions, particle.x, particle.y);
                particle.lifetime = -1.0f; // Mark the particle for removal
                break; // Exit the loop once a collision is detected
            }
        }
    }

    // Remove particles that expire or collide with any planet
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& particle) {
            return particle.lifetime <= 0.0f;
        }),
        particles.end());
}


void UpdateExplosions(std::vector<Explosion>& explosions, float deltaTime) {
    for (auto& explosion : explosions) {
        explosion.lifetime -= deltaTime;
        explosion.size *= 0.95f; // Gradually shrink the explosion
                                                                            
        // Generate random shake offsets in range [-shakeIntensity, +shakeIntensity]
        explosion.shakeOffsetX = ((rand() % 100) / 100.0f) * explosion.shakeIntensity * 2 - explosion.shakeIntensity;
        explosion.shakeOffsetY = ((rand() % 100) / 100.0f) * explosion.shakeIntensity * 2 - explosion.shakeIntensity;
    }
    // Remove expired explosions
    explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
        [](const Explosion& e) { return e.lifetime <= 0.0f; }),
        explosions.end());
}


void RenderParticles(std::vector<Particle>& particles, AEGfxVertexList* particleMesh, Camera* camera) {
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.0f);

    // Get the camera's zoom level
    float zoom = camera->cameraZoom;  // Camera zoom level
    AEVec2 cameraPosition = camera->position; // Camera position (for adjusting positions accordingly)

    for (Particle& particle : particles) {
        // Apply the camera zoom to both the particle position and size
        // Adjust particle's position relative to the camera's position and apply zoom
        float zoomedX = (particle.x - cameraPosition.x) * zoom + cameraPosition.x;
        float zoomedY = (particle.y - cameraPosition.y) * zoom + cameraPosition.y;

        // Apply zoom to the size of the particle (optional, depending on how you want to scale)
        float zoomedSize = 5.0f * zoom;  // Base size of 5.0f, scaled by the camera zoom

        // Generate the transform matrix with zoom applied to the particle's position and size
        AEMtx33 transform = GetTransform(zoomedX, zoomedY, zoomedSize, zoomedSize);

        // Set the transformation matrix
        AEGfxSetTransform(transform.m);

        // Draw the particle mesh
        AEGfxMeshDraw(particleMesh, AE_GFX_MDM_TRIANGLES);
    }
}

void RenderExplosions(const std::vector<Explosion>& explosions, AEGfxVertexList* explosionMesh, Camera* camera) {
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(0.8f); // Explosions are slightly transparent

    // Get the camera's zoom level and position
    float zoom = camera->cameraZoom;
    AEVec2 cameraPosition = camera->position;

    for (const auto& explosion : explosions) {
        // Adjust explosion position relative to the camera
        float adjustedX = (explosion.x + explosion.shakeOffsetX - cameraPosition.x) * zoom;
        float adjustedY = (explosion.y + explosion.shakeOffsetY - cameraPosition.y) * zoom;

        // Adjust explosion size based on zoom
        float explosionScale = explosion.size * zoom;

        // Create the transformation matrix
        AEMtx33 transform = GetTransform(adjustedX, adjustedY, explosionScale, explosionScale);
        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(explosionMesh, AE_GFX_MDM_TRIANGLES);
    }
}

