/******************************************************************************/
/**
 * @file        CreditsScreen.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Implementation of the credits screen, which displays credits
 *              and contributors to the game.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include <cmath>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/CreditsScreen.hpp"
#include "../Header/Stars.hpp"
#include "../Header/Game.hpp"
#include "../Header/UIManager.hpp"

extern int next;

float creditsY = 600.0f; // Start position below the screen
float scrollSpeed = 300.0f; // Pixels per second
float screenHeight = 900.0f;
float lineSpacing = 60.0f / screenHeight;
f32 Padding = 0.05f;

const char* creditsText[] = {
    "All content copyrighted 2025 DigiPen Institute of Technology Singapore. All Rights Reserved", "WWW.DIGIPEN.EDU",
    "", "", //'\n'
    "The Astral Seige", 
    "", "", 
    "Made by: The Alpha Beasts", 
    "", "",
    "Choi Meng Yew", "Javier Lee", "Guok Yi Yong", "Chloe Lau Rey En", "Celeste Tong Jia Xuan", "", "OUR TEAM",
    "", "",
    "Cheng Ding Xiang", "Gerald Wong", "Jeremy Chew Ping", "Tan Chee Wei Tommy", "Soroor MALEKMOHAMMADI FARADOUNBEH", "", "FACULTY AND ADVISORS",
    "", "",
    "JASON CHU", "SAMIR ABOU SAMRA", "MICHELE COMAIR", "ANGELA KUGLER", "ERIK MOHRMANN", "BENJAMIN ELLINGER", "MELVIN GONSALVEZ", "", "EXECUTIVES",
    "", "",
    "CLAUDE COMAIR","", "PRESIDENT",
    "", "",
    "DigiPen Institute of Technology Singapore", "Created at",
    "", "",
    "Spaceship: https://opengameart.org/content/spaceship-9", "Victory Sound Effect Composed by: Jon K. Fite", "Purchasing Sound Effect Composed by: Spring Spring", "Sound Effects SFX Composed by: Jesús Lastra", "Turret and bullets by: Ansimuz", "Missile Launcher by: Zintoki", "UI: Kenney", "", "Copyright:",
};

int numLines = sizeof(creditsText) / sizeof(creditsText[0]);


void Credits_Load() {
    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();

    // create and initialize the UI manager
    uiManager = new UIManager();
    uiManager->InitializeFonts();
}

void Credits_Initialize() {
    // Create star meshes first
    starMesh = createCircleMesh(0xFFFFFFFF);
    supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

    InitializeStars(); // Initialize the star field
    ShowConfirmation = false;
}

void Credits_Update() {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    UpdateStars(dt); // Update star positions

    f32 textWidth = 0, textHeight = 0;
    creditsY -= scrollSpeed * dt; // Scroll upwards

    // Calculate the total height of the credits text in pixels
    for (int i = 0; i < numLines; ++i) {
        AEGfxGetPrintSize(UIManager::GetFontId(), creditsText[i], 1.0f, &textWidth, &textHeight);
    }
    f32 totalCreditsHeight = (screenHeight * textHeight * numLines) + (lineSpacing * screenHeight * numLines) - (Padding * numLines); // lineSpacing is in normalized coordinates

    // Reset creditsY if all text has scrolled past the bottom of the screen
    if ( creditsY < -totalCreditsHeight) {
        creditsY = 600.0f; // Reset to start below the screen
    }

    BackButton_Update();
    PopUp_Update();
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        ShowConfirmation = !ShowConfirmation;
    }
}

void Credits_Draw() {
    AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);
    RenderStars(starMesh, supernovaStarMesh);

    // Draw the title "CREDITS" at the top
    uiManager->DrawTitle("CREDITS");

    // Calculate the starting position of the credits text based on the title's position
    f32 targetY = 0.75f; // This should match the targetY used in DrawTitle
    f32 titleWidth = 0, titleHeight = 0;
    AEGfxGetPrintSize(UIManager::GetFontId(), "CREDITS", 1.5f, &titleWidth, &titleHeight);
    f32 titleBottomY = targetY - titleHeight - Padding; // Position below the title


    // Draw the credits text starting from creditsStartY
    float yPos = creditsY / screenHeight;
    for (int i = 0; i < numLines; ++i) {
        f32 textWidth = 0, textHeight = 0;
        AEGfxGetPrintSize(UIManager::GetFontId(), creditsText[i], 1.0f, &textWidth, &textHeight);
        float leftX = -textWidth / 2.0f; // Center text horizontally

        // Only draw the text if it's below the title
        if (yPos < titleBottomY) {
            AEGfxPrint(UIManager::GetFontId(), creditsText[i], leftX, yPos,
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
        yPos += lineSpacing; // Move to next line
    }
    BackButton_Draw();
}

void Credits_Free() {
    MeshManager::GetInstance().UnloadTexture();
    MeshManager::GetInstance().UnloadMesh();

    AEGfxMeshFree(starMesh);
    AEGfxMeshFree(supernovaStarMesh);
}

void Credits_Unload() {
    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;
}