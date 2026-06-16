/******************************************************************************/
/**
 * @file: CurrencyManager.hpp
 * @project: Astral Siege
 * @author: Chloe Lau Rey En (primary)
 * @brief: This file handles the player character movement and collision detection
 * @copyright: All content © 2025 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "../Header/CoinTextAnimation.hpp"
#include "../Header/CoinAnimation.hpp"
#include "../Header/UIManager.hpp"
#include "../Header/camera.hpp"
#include "AEEngine.h"

class CurrencyManager {
private:
    static CurrencyManager* instance;
    int currency;
    bool gameOver; // Flag to stop updates when player dies
    s8 font;
    CoinAnimation* coinAnimation;
    CoinTextAnimation* coinTextAnimation;

    bool displayCollisionText = false;
    float collisionTextTime = 0.0f;
    const float collisionTextDuration = 5.0f;
    int collisionTextAmount;

    CurrencyManager();

public:
    static CurrencyManager& GetInstance();
    static void DestroyInstance();

    void LoadFont();
    void Update(float dt);
    void ShowCollisionText(int amount);
    void IncrementCurrency(int amount); // Made it so that it accepts an amount parameter
    void DeductCurrency(int amount);
    void Draw();
    void Reset();
    int GetCurrency() const;
    void SetCurrency(int value);
    void SetGameOver(bool state); // Stop updates when player is dead

    void AddRefund(int amount); // overloaded function for sell button
    CoinTextAnimation* GetCoinTextAnimation() {
        return coinTextAnimation;
    }
};
