/******************************************************************************/
/**
 * @file        AudioManager.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file handles the player character movement and collision detection
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
                All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEAudio.h"
#include <unordered_map>
#include <string>
#include "AudioConfigLoader.hpp"

class AudioManager {
private:
    // Singleton instance
    static AudioManager* instance;

    // Audio groups
    AEAudioGroup bgmGroup;
    AEAudioGroup sfxGroup;

    // Currently playing background music
    AEAudio currentBGM;

    // Maps to store loaded audio files
    std::unordered_map<std::string, AEAudio> audioMap;
    std::unordered_map<std::string, bool> isMusicMap;
    std::unordered_map<std::string, float> volumeMap;
    std::unordered_map<std::string, float> pitchMap;

    bool bgMusicInitialized = false;

    // Private constructor for singleton pattern
    AudioManager();

    float bgmVolume = 0.5f;
    float sfxVolume = 1.0f;
    float masterVolume;

public:
    ~AudioManager();

    // Get singleton instance
    static AudioManager& GetInstance();
    static void DestroyInstance();

    // Core functionality
    void Initialize();
    void Shutdown();

    // Initialize default audio
    void InitializeDefaultAudio(bool playBGMusic);

    // Load audio from JSON config
    void LoadAudioConfigs(const std::string& configFile);

    // Load individual audio file
    void LoadAudio(const std::string& name, const std::string& filepath, bool isMusic,
        float defaultVolume = 1.0f, float defaultPitch = 1.0f);

    // Play audio
    void PlayMusic(const std::string& name, bool loop = true);
    void PlaySFXSound(const std::string& name);

    // Set master volume
    void SetMasterVolume(float volume);

    // Stop a specific audio (useful for sound effects that might be looping)
    void StopAudio(const std::string& name);

    // Volume control
    void SetVolume(const std::string& name, float volume);
    void SetGroupVolume(bool isMusic, float volume);

    // Pitch control
    void SetPitch(const std::string& name, float pitch);
    void SetGroupPitch(bool isMusic, float pitch) const;

    // Pause/Resume functionality
    void PauseMusic() const;
    void ResumeMusic() const;

    // Stop all audio
    void StopAll();

    void StopSFXOnly() const;

    void InitializeBackgroundMusic();
    bool IsBgMusicInitialized() const { return bgMusicInitialized; }

    float GetBGMVolume() const {
        return bgmVolume;
    }

    float GetSFXVolume() const {
        return sfxVolume;
    }

    float GetMasterVolume() const {
        return masterVolume;
    }

    AEAudioGroup GetBGMGroup() const { return bgmGroup; }
    AEAudioGroup GetSFXGroup() const { return sfxGroup; }

    AEAudio GetCurrentBGM() const { return currentBGM; }

    static bool HasInstance() { return instance != nullptr; }
};