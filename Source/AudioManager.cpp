/******************************************************************************/
/**
 * @file        UIManager.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (Primary)
 * @brief       Implements the UIManager class responsible for rendering and managing UI elements.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include <fstream>
#include "AEEngine.h"
#include "../Header/AudioManager.hpp"
#include "../Header/UIManager.hpp"

// Initialize static instance pointer to null
AudioManager* AudioManager::instance = nullptr;
static bool isShutdown = false;

// Destructor
AudioManager::~AudioManager() {
    Shutdown();
}

// Constructor: Initialize audio groups
AudioManager::AudioManager() : currentBGM({ nullptr }), bgmVolume(1.0f), sfxVolume(1.0f), masterVolume(1.0f) {
    bgmGroup = AEAudioCreateGroup();    // Create group for background music
    sfxGroup = AEAudioCreateGroup();    // Create group for sound effects
}

// Singleton pattern: Get the AudioManager instance
AudioManager& AudioManager::GetInstance() {
    if (instance == nullptr) {
        instance = new AudioManager();  // Create new instance if none exists
    }
    return *instance;
}

// Clean up the AudioManager singleton
void AudioManager::DestroyInstance() {
    if (instance) {
        instance->Shutdown();  // Make sure we call Shutdown first
        delete instance;
        instance = nullptr;
    }
}

// Initialize audio system
void AudioManager::Initialize() {
    // AEAudioInit() is called in AESysInit, so we don't need to call it here
}

// Load audio configurations from a JSON file
void AudioManager::LoadAudioConfigs(const std::string& configFile) {
    std::vector<AudioConfig> configs = loadAudioConfigs(configFile);  // Parse config file

    // Load each audio file based on configuration
    for (const auto& config : configs) {
        LoadAudio(config.name, config.filepath, config.isMusic,
            config.defaultVolume, config.defaultPitch);
    }
}

// Initialize default audio settings and optionally start background music
void AudioManager::InitializeDefaultAudio(bool playBGMusic) {
    // Initialize audio
    Initialize();

    // Load configs from default location using FindFilePath
    LoadAudioConfigs("Assets/config/audio_configs.json");

    // Apply stored volume settings
    AEAudioSetGroupVolume(bgmGroup, bgmVolume);
    AEAudioSetGroupVolume(sfxGroup, sfxVolume);

    // Start background music if requested
    if (playBGMusic) {
        PlayMusic("bgm_mainmenu", true);  // Play main menu music with looping
    }
}

// Release all audio resources
void AudioManager::Shutdown() {
    if (isShutdown) return;
    StopAll();  // Stop all playing audio

    // Unload all audio files
    for (auto& pair : audioMap) {
        if (AEAudioIsValidAudio(pair.second)) {
            AEAudioUnloadAudio(pair.second);
        }
    }

    // Clear all maps (use proper C++ clearing)
    audioMap.clear();
    isMusicMap.clear();
    volumeMap.clear();
    pitchMap.clear();

    // Unload audio groups
    if (AEAudioIsValidGroup(bgmGroup)) {
        AEAudioUnloadAudioGroup(bgmGroup);
        bgmGroup = { nullptr };
    }
    if (AEAudioIsValidGroup(sfxGroup)) {
        AEAudioUnloadAudioGroup(sfxGroup);
        sfxGroup = { nullptr };
    }

    currentBGM = { nullptr };
    isShutdown = true;
}

// Load an audio file with associated metadata
void AudioManager::LoadAudio(const std::string& name, const std::string& filepath,
    bool isMusic, float defaultVolume, float defaultPitch) {
    if (audioMap.find(name) != audioMap.end()) {
        return;  // Skip if already loaded (prevent duplicates)
    }

    // Load as either music or sound effect
    AEAudio audio = isMusic ?
        AEAudioLoadMusic(filepath.c_str()) :  // Music typically streamed from disk
        AEAudioLoadSound(filepath.c_str());   // Sound effects loaded into memory

    if (AEAudioIsValidAudio(audio)) {
        // Store audio and its properties if successfully loaded
        audioMap[name] = audio;
        isMusicMap[name] = isMusic;
        volumeMap[name] = defaultVolume;
        pitchMap[name] = defaultPitch;
    }
}

// Play background music, stopping any currently playing BGM
void AudioManager::PlayMusic(const std::string& name, bool loop) {
    auto it = audioMap.find(name);
    if (it == audioMap.end() || !isMusicMap[name]) return;

    if (AEAudioIsValidAudio(currentBGM)) {
        AEAudioStopGroup(bgmGroup);
    }

    currentBGM = it->second;

    // Apply the current bgmVolume (not the default volume for this track)
    AEAudioPlay(currentBGM, bgmGroup, bgmVolume, pitchMap[name], loop ? -1 : 0);

    // Make absolutely sure the group volume is applied
    AEAudioSetGroupVolume(bgmGroup, bgmVolume);

    // If volume is zero or very low, ensure the audio is muted
    if (bgmVolume <= 0.01f) {
        AEAudioSetGroupVolume(bgmGroup, 0.0f);
    }
}

// Play a sound effect once
void AudioManager::PlaySFXSound(const std::string& name) {
    auto it = audioMap.find(name);
    if (it == audioMap.end() || isMusicMap[name]) return;  // Check if exists and is SFX

    // Play sound (no loop, plays once)
    AEAudioPlay(it->second, sfxGroup, volumeMap[name], pitchMap[name], 0);
}

// Stop a specific audio by name
void AudioManager::StopAudio(const std::string& name) {
    auto it = audioMap.find(name);
    if (it == audioMap.end()) return;  // Check if exists

    // Stop the specific audio's group
    AEAudioStopGroup(isMusicMap[name] ? bgmGroup : sfxGroup);

    // Reset current BGM reference if we stopped the active BGM
    if (isMusicMap[name] && it->second.fmod_sound == currentBGM.fmod_sound) {
        currentBGM = { nullptr };
    }
}

// Set volume for a specific audio
void AudioManager::SetVolume(const std::string& name, float volume) {
    auto it = audioMap.find(name);
    if (it == audioMap.end()) return;  // Check if exists

    // Store the new volume
    volumeMap[name] = volume;

    // Update active BGM volume if this is the current BGM
    if (isMusicMap[name] && it->second.fmod_sound == currentBGM.fmod_sound) {
        AEAudioSetGroupVolume(bgmGroup, volume);
    }
}

void AudioManager::SetGroupVolume(bool isSFX, float volume) {
    // Store volume in member variable
    if (isSFX) {
        sfxVolume = volume;
    }
    else {
        bgmVolume = volume;
    }

    // Apply volume
    AEAudioSetGroupVolume(isSFX ? sfxGroup : bgmGroup, volume * masterVolume);
}

// Set pitch for a specific audio
void AudioManager::SetPitch(const std::string& name, float pitch) {
    auto it = audioMap.find(name);
    if (it == audioMap.end()) return;  // Check if exists

    // Store the new pitch
    pitchMap[name] = pitch;

    // Update active BGM pitch if this is the current BGM
    if (isMusicMap[name] && it->second.fmod_sound == currentBGM.fmod_sound) {
        AEAudioSetGroupPitch(bgmGroup, pitch);
    }
}

// Set pitch for all music or all sound effects
void AudioManager::SetGroupPitch(bool isMusic, float pitch) const {
    // Apply pitch to either music or SFX group
    AEAudioSetGroupPitch(isMusic ? bgmGroup : sfxGroup, pitch);
}

// Pause currently playing background music
void AudioManager::PauseMusic() const {
    if (AEAudioIsValidAudio(currentBGM)) {
        AEAudioPauseGroup(bgmGroup);  // Pause the entire BGM group
    }
}

// Resume previously paused background music
void AudioManager::ResumeMusic() const {
    if (AEAudioIsValidAudio(currentBGM)) {
        AEAudioResumeGroup(bgmGroup);  // Resume the entire BGM group
    }
}

// Stop all audio playback
void AudioManager::StopAll() {
    // Stop both BGM and SFX groups
    if (AEAudioIsValidGroup(bgmGroup)) {
        AEAudioStopGroup(bgmGroup);
    }
    if (AEAudioIsValidGroup(sfxGroup)) {
        AEAudioStopGroup(sfxGroup);
    }
    // Reset current BGM reference
    currentBGM = { nullptr };
}

void AudioManager::InitializeBackgroundMusic() {
    // Check if the music exists in your audioMap
    auto it = audioMap.find("gameTheme");
    //if (it == audioMap.end()) {
    //    std::cout << "ERROR: 'gameTheme' not found in audioMap!" << std::endl;
    //}
    //else {
    //    std::cout << "Found 'gameTheme' in audioMap." << std::endl;
    //}
    if (!bgMusicInitialized) {
        PlayMusic("gameTheme", true);
        bgMusicInitialized = true;
    }
}

void AudioManager::StopSFXOnly() const {
    // Stop only the SFX group
    if (AEAudioIsValidGroup(sfxGroup)) {
        AEAudioStopGroup(sfxGroup);
    }
}

void AudioManager::SetMasterVolume(float volume) {
    masterVolume = volume;
    // Apply master volume to both groups
    AEAudioSetGroupVolume(bgmGroup, bgmVolume * masterVolume);
    AEAudioSetGroupVolume(sfxGroup, sfxVolume * masterVolume);
}