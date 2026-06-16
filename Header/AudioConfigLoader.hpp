/******************************************************************************/
/**
 * @file        AudioConfigLoader.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file defines audio configuration data and provides a
 *              loader for audio settings from external files.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include <string>
#include <vector>

struct AudioConfig {
    std::string name;        // Internal name for the audio
    std::string filepath;    // Path to the audio file
    bool isMusic;            // Whether it's background music or a sound effect
    float defaultVolume;     // Default volume level
    float defaultPitch;      // Default pitch level

    AudioConfig() :
        name(""),
        filepath(""),
        isMusic(false),
        defaultVolume(1.0f),
        defaultPitch(1.0f) 
    {}
};

std::vector<AudioConfig> loadAudioConfigs(const std::string& filename);