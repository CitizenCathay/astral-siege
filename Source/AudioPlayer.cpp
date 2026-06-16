/******************************************************************************/
/**
 * @file        AudioPlayer.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file handles the player character movement and collision detection
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
                All rights reserved.
 */
 /******************************************************************************/

#include "../Header/AudioPlayer.hpp"
#include "../Header/AudioConfigLoader.hpp"
#include "../Header/UIManager.hpp"

// Initialize the static instance pointer
AudioPlayer* AudioPlayer::instance = nullptr;

// Constructor - loads sound configurations
AudioPlayer::AudioPlayer() {
    // Load audio settings from config file
    LoadVolumeSettings();
}

// Load volume settings from configuration file
void AudioPlayer::LoadVolumeSettings(const std::string& configFile) {
    std::vector<AudioConfig> configs = loadAudioConfigs(configFile);

    // Set volumes based on config
    for (const auto& config : configs) {
        // Store volumes locally
        soundVolumes[config.name] = config.defaultVolume;

        // Apply volumes to AudioManager
        AudioManager::GetInstance().SetVolume(config.name, config.defaultVolume);
    }
}

// Singleton instance getter
AudioPlayer& AudioPlayer::GetInstance() {
    if (!instance) {
        instance = new AudioPlayer();
    }
    return *instance;
}

// Destroy the singleton instance
void AudioPlayer::DestroyInstance() {
    if (instance) {
        // Clear any internal resources
        instance->soundVolumes.clear();
        
        delete instance;
        instance = nullptr;
    }
}

// Sound effect methods

void AudioPlayer::PlayEnemyDestroyed() {
    AudioManager::GetInstance().PlaySFXSound("sfx_coin_collect");
    AudioManager::GetInstance().PlaySFXSound("sfx_kill_enemy");
}

void AudioPlayer::PlayWeaponShoot() {
    AudioManager::GetInstance().PlaySFXSound("sfx_weapon_shoot");
}

void AudioPlayer::PlayVictory() {
    AudioManager::GetInstance().PlaySFXSound("sfx_win");
}

void AudioPlayer::PlayDefeat() {
    AudioManager::GetInstance().PlaySFXSound("sfx_lose");
}

void AudioPlayer::PlayTakeDamage() {
    AudioManager::GetInstance().PlaySFXSound("sfx_take_damage");
}

void AudioPlayer::PlayPurchase() {
    AudioManager::GetInstance().PlaySFXSound("sfx_purchase");
}

// Play a sound by name
void AudioPlayer::PlaySpecSound(const std::string& soundName) {
    AudioManager::GetInstance().PlaySFXSound(soundName);
}

// Set sound volume
void AudioPlayer::SetSoundVolume(const std::string& soundName, float volume) {
    soundVolumes[soundName] = volume;
    AudioManager::GetInstance().SetVolume(soundName, volume);
}

// Get sound volume
float AudioPlayer::GetSoundVolume(const std::string& soundName) const {
    auto it = soundVolumes.find(soundName);
    if (it != soundVolumes.end()) {
        return it->second;
    }
    return 1.0f; // Default volume if not found
}