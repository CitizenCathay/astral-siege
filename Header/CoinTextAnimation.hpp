/******************************************************************************/
/**
 * @file        CoinTextAnimation.hpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary)
 * @brief       This file implements the coin popup animation system
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "AEEngine.h"
#include "../Header/UIManager.hpp"
#include <vector>
#include <string>

// Text animation for showing coin values when collected
class CoinTextAnimation {
private:
    struct TextPopup {
        AEVec2 position = { 0.0f, 0.0f };    // Current position
        AEVec2 velocity = { 0.0f, 0.0f };    // Movement speed and direction
        std::string text;                   // Text to display (e.g., "+10")
        float alpha = 0.0f;                 // Opacity (1.0 = fully visible, 0.0 = invisible)
        float lifetime = 0.0f;              // How long the popup has existed
        float maxLifetime = 1.0f;           // When to remove the popup
        float scale = 1.0f;                 // Text size multiplier
    };

    std::vector<TextPopup> activePopups;
    int fontId;

public:
    CoinTextAnimation() : fontId(-1) {}

    // Initialize the text animation system
    void Initialize() {
        // We'll use the same font as CurrencyManager
        fontId = -1; // Will be set to UIManager font
    }

    // Update all active text popups
    void Update() {
        float dt = (float)AEFrameRateControllerGetFrameTime();

        // Update all active popups
        for (size_t i = 0; i < activePopups.size(); ++i) {
            TextPopup& popup = activePopups[i];

            // Move upward
            popup.position.x += popup.velocity.x * dt;
            popup.position.y += popup.velocity.y * dt;

            // Fade out over time
            popup.lifetime += dt;
            float normalizedLife = popup.lifetime / popup.maxLifetime;

            // First half of lifetime - fade in, second half - fade out
            if (normalizedLife < 0.2f) {
                popup.alpha = normalizedLife / 0.2f; // Fade in quickly
            }
            else {
                popup.alpha = 1.0f - ((normalizedLife - 0.2f) / 0.8f); // Fade out more slowly
            }

            // Remove if expired
            if (popup.lifetime >= popup.maxLifetime) {
                activePopups.erase(activePopups.begin() + i);
                --i;
            }
        }
    }

    // Draw all active text popups
    void Draw() {
        if (fontId == -1) {
            fontId = UIManager::GetFontId();
        }

        // Save current camera position for restoration later
        f32 oldCamX, oldCamY;
        AEGfxGetCamPosition(&oldCamX, &oldCamY);

        // Reset camera for UI elements
        AEGfxSetCamPosition(0, 0);

        // Draw all popups
        for (const auto& popup : activePopups) {
            // Draw text with current alpha and scale
            // Make sure we're passing the text from the popup, not a hardcoded value
            AEGfxPrint(UIManager::GetFontId(), popup.text.c_str(), popup.position.x, popup.position.y,
                popup.scale, 1.0f, 0.8f, 0.0f, popup.alpha);
        }

        // Restore camera position
        AEGfxSetCamPosition(oldCamX, oldCamY);
    }

    // Create a new text popup
    void AddPopup(float x, float y, int value, float duration = 1.5f) {
        TextPopup popup;
        popup.position = { x, y };
        popup.velocity = { 0.0f, 0.2f }; // Move upward
        popup.text = "+" + std::to_string(value);
        popup.alpha = 0.0f; // Start transparent
        popup.lifetime = 0.0f;
        popup.maxLifetime = duration;
        popup.scale = 1.2f;

        activePopups.push_back(popup);
    }
};