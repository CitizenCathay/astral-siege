/******************************************************************************/
/**
 * @file        AudioConfigLoader.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (Primary)
 * @brief       Implementation of the configuration of stored audio files.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/AudioConfigLoader.hpp"
#include "../Header/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

std::vector<AudioConfig> loadAudioConfigs(const std::string& filename) {
    std::vector<AudioConfig> audioConfigs;
    std::ifstream file{ filename };
    if (!file.is_open()) {
        std::cerr << "Could not open audio JSON file: " << filename << std::endl;
        return audioConfigs;
    }

    json j;
    file >> j;

    for (const auto& audio : j["audio"]) {
        AudioConfig config;
        config.name = audio["name"].get<std::string>();
        config.filepath = audio["filepath"].get<std::string>();
        config.isMusic = audio["isMusic"].get<bool>();
        config.defaultVolume = audio.contains("defaultVolume") ?
            audio["defaultVolume"].get<float>() : 1.0f;
        config.defaultPitch = audio.contains("defaultPitch") ?
            audio["defaultPitch"].get<float>() : 1.0f;
        audioConfigs.push_back(config);
    }
    return audioConfigs;
}