/******************************************************************************/
/**
 * @file        CurrencyManager.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (Primary)
 * @brief       This file handles the player character movement and collision detection
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/CurrencyManager.hpp"
#include "AEGraphics.h"
#include <iostream>

CurrencyManager* CurrencyManager::instance = nullptr;

CurrencyManager::CurrencyManager() :
    currency(300),
    gameOver(false),
    font(-1),
    coinAnimation(nullptr),
    coinTextAnimation(nullptr),
    displayCollisionText(false),
    collisionTextTime(0.0f),
    collisionTextAmount(0)
{
    coinAnimation = new CoinAnimation();
    coinAnimation->Initialize();

    coinTextAnimation = new CoinTextAnimation();
    coinTextAnimation->Initialize();
}

CurrencyManager& CurrencyManager::GetInstance() {
    if (!instance) {
        instance = new CurrencyManager();
        instance->LoadFont();
    }
    return *instance;
}

void CurrencyManager::DestroyInstance() {
    if (instance) {
        delete instance->coinAnimation;
        delete instance->coinTextAnimation;
        delete instance;
        instance = nullptr;
    }
}

void CurrencyManager::LoadFont() {
    font = UIManager::GetFontId();
}

void CurrencyManager::Update(float) {
    if (gameOver) return; // Stop updating if the game is over

    if (coinAnimation) {
        coinAnimation->Update();
    }

    if (coinTextAnimation) {
        coinTextAnimation->Update();
    }

    // Update collision text timer if active
    if (displayCollisionText) {
        collisionTextTime += (float)AEFrameRateControllerGetFrameTime();
        if (collisionTextTime >= collisionTextDuration) {
            displayCollisionText = false;
        }
    }
}

void CurrencyManager::IncrementCurrency(int amount) {
    if (gameOver) return; // Stop updating if the game is over
    currency += amount; // Use the provided amount
}

void CurrencyManager::AddRefund(int amt) {
    if (gameOver) return; // Stop updating if the game is over
    currency += amt; // refunds each turret
}

void CurrencyManager::DeductCurrency(int amount) {
    if (amount <= 0) return; // Invalid amount

    // Ensure we don't go below zero
    if (currency >= amount) {
        currency -= amount;
    }
}

void CurrencyManager::Draw() {
    // Text positioning for bottom right
    float textX = 0.71f;  // Right side of screen
    float textY = -0.90f; // Bottom of screen

    // Draw currency text first
    char buffer[32];
    sprintf_s(buffer, sizeof(buffer), "%d x", currency);
    AEGfxPrint(UIManager::GetFontId(), buffer, textX, textY, 1.5f, 1.0f, 1.0f, 0.0f, 1.0f);

    // Draw coin after the text
    if (coinAnimation) {
        float coinX = 735.0f;  // Right side
        float coinY = -380.0f; // Bottom

        // Draw only the coin
        coinAnimation->DrawCoinOnly(coinX, coinY, 90.0f);
    }

    if (coinTextAnimation) {
        coinTextAnimation->Draw();
    }
}

void CurrencyManager::Reset() {
    currency = 70; // Reset currency to 100
    gameOver = false; // Allow updates again
}

int CurrencyManager::GetCurrency() const {
    return currency;
}

void CurrencyManager::SetCurrency(int value) {
    currency = value;
}

void CurrencyManager::SetGameOver(bool state) {
    gameOver = state;
}

void CurrencyManager::ShowCollisionText(int amount) {
    if (coinTextAnimation) {
        // Get position near coin counter in bottom left
        float x = 0.7f;
        float y = -0.7f;

        // Add popup with the actual amount - make sure this value is passed correctly
        coinTextAnimation->AddPopup(x, y, amount);
    }
}