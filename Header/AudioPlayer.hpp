/******************************************************************************/
/**
 * @file        AudioPlayer.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file handles the player character movement and collision detection
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
                All rights reserved.
 */
 /******************************************************************************/

#pragma once
#include "../Header/AudioManager.hpp"
#include <string>
#include <unordered_map>

class AudioPlayer {
private:
    static AudioPlayer* instance;

    // Store volume configurations
    std::unordered_map<std::string, float> soundVolumes;

    // Private constructor for singleton pattern
    AudioPlayer();

    // Load volumes from configuration
    void LoadVolumeSettings(const std::string& configFile = "Assets/config/audio_configs.json");

public:
    // Singleton pattern methods
    static AudioPlayer& GetInstance();
    static void DestroyInstance();

    // Sound playback methods
    void PlayEnemyDestroyed();
    void PlayWeaponShoot();
    void PlayVictory();
    void PlayDefeat();
    void PlayTakeDamage();
    void PlayPurchase();

    // Generic sound playback method
    void PlaySpecSound(const std::string& soundName);

    // Volume control
    void SetSoundVolume(const std::string& soundName, float volume);
    float GetSoundVolume(const std::string& soundName) const;
};