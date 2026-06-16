/******************************************************************************/
/**
 * @file        MainMenu.cpp
 * @project     Astral Siege
 * @author      Chloe Lau Rey En (primary) - 50%
 * @author      Celeste Tong Jia Xuan (secondary) - update main menu (50%)
 * @brief       This file manages the main menu state and its UI elements
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
                All rights reserved.
 */
 /******************************************************************************/

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "../Header/MainMenu.hpp"
#include "../Header/Stars.hpp"
#include "../Header/Game.hpp"
#include "../Header/UIManager.hpp"
#include "../Header/MainMenuObjects.hpp"

AEGfxVertexList* menuButtonMesh = nullptr;
AEGfxVertexList* earthSpriteMesh = nullptr;  // For background image
AEGfxTexture* earthSpriteTexture = nullptr;  // For background image

static std::vector<float> buttonColor;
static std::vector<float> buttonScale;
static const float DEFAULT_SCALE = 1.0f;
static const float HOVER_SCALE = 1.3f; // 30% larger when hovered
static float earthRotation = 0.0f;  // Tracks earth rotation angle in radians

// Color constants
static const float DEFAULT_COLOR = 0.5f;  // White default
static const float HOVER_COLOR = 1.0f;    // Yellow when hovered

const float WHITE_R = 1.0f;
const float WHITE_G = 1.0f;
const float WHITE_B = 1.0f;

const float YELLOW_R = 1.0f;
const float YELLOW_G = 1.0f;
const float YELLOW_B = 0.0f;

UIManager* uiManager = nullptr;
float scaleFactor = 1.0f;
extern int next;

// Linear interpolation function for smooth scaling
float Lerp(float currentVal, float target, float speed, float dt) {
    return currentVal + (target - currentVal) * speed * dt;
}

void Menu_Load() {
    // Initialize the ship
    MainMenuObjects::Initialize();

	// Creates button mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f
	);
	menuButtonMesh = AEGfxMeshEnd();

	// for future earth sprite to put in the middle(have to chagne coordinates)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	earthSpriteMesh = AEGfxMeshEnd();

    // Load background texture
	earthSpriteTexture = AEGfxTextureLoad("Assets/earth.png");

	// create and initialize the UI manager
	uiManager = new UIManager();
	uiManager->InitializeFonts();

    // Resize buttonColor to match the number of buttons and initialize to white (0.5f)
    buttonColor.resize(5, 0.5f);
    buttonScale.resize(5, DEFAULT_SCALE);
}

void Menu_Initialize() {
	// Create star meshes first
	starMesh = createCircleMesh(0xFFFFFFFF);
	supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

	InitializeStars(); // Initialize the star field

    // Get current window dimensions
	AEGfxGetWindowHeight();
	AEGfxGetWindowWidth();

    scaleFactor = 1.2f; // Scale UI by 20%

    // Load the audio configs explicitly
    AudioManager::GetInstance().LoadAudioConfigs("Assets/config/audio_configs.json");

    // Store current volume settings before initializing music
    float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
    float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

    // Check if background music is initialized
    if (!AudioManager::GetInstance().IsBgMusicInitialized()) {
        AudioManager::GetInstance().InitializeBackgroundMusic();
    }

    // Explicitly set volumes to preserve settings
    AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);  // SFX volume
    AudioManager::GetInstance().SetGroupVolume(false, bgmVolume); // BGM volume

    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();
    ShowQuitConfirmation = false;
}

void Menu_Update() {
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	UpdateStars(dt); // Update star positions

    // Update Earth rotation - adjust rotation speed as needed
    const float ROTATION_SPEED = 0.01f;  // radians per second
    earthRotation += ROTATION_SPEED * dt;

    // Keep rotation angle within 0-2pi range for better numerical stability
    if (earthRotation > PI * 2.0f) {
        earthRotation -= PI * 2.0f;
    }

    // Update ship movement
    MainMenuObjects::Update(dt);

    // Reset all buttons to default state BEFORE checking hover
    for (int i = 0; i < buttonColor.size(); i++) {
        buttonColor[i] = 0.5f; // Default white
        buttonScale[i] = Lerp(buttonScale[i], DEFAULT_SCALE, 5.0f, dt);
    }

    if (!ShowQuitConfirmation) {
        s32 mouseX, mouseY;
        AEInputGetCursorPosition(&mouseX, &mouseY);

        // convert mouse position to normalized space
        float normX, normY;
        UIManager::ScreenToNormalized(mouseX, mouseY, normX, normY);

        // Position both columns on the right side of earth
        float columnSpacing = 150.0f;  // Reduced spacing between columns
        float leftStartX = 125.0f;    // Moved first column more to the right
        float rightStartX = leftStartX + columnSpacing;  // Second column position adjusts automatically
        float startY = 60.0f;
        float verticalSpacing = 90.0f;

        // Two-column menu layout on the right side of earth
        const char* leftColumnLabels[] = { "START GAME", "CREDITS" };
        const char* rightColumnLabels[] = { "CONTROLS", "OPTIONS" };
        const int columnButtons = 2;

        // Check left column
        for (int i = 0; i < columnButtons; i++) {
            float y = startY - (i * verticalSpacing);

            // Check if mouse clicks left column button
            float normLeftX = leftStartX / 400.0f - 0.2f;
            float normLeftY = y / 300.0f;

            // Get the width and height of the button text already normalized
            f32 lefttextWidth = 0, lefttextHeight = 0;
            AEGfxGetPrintSize(UIManager::GetFontId(), leftColumnLabels[i], 1.0f, &lefttextWidth, &lefttextHeight);

            if (normX >= normLeftX && normX <= normLeftX + lefttextWidth &&
                normY >= normLeftY && normY <= normLeftY + lefttextHeight) {
                buttonColor[i] = HOVER_COLOR; // Change to yellow when hovered
                buttonScale[i] = Lerp(buttonScale[i], HOVER_SCALE, 4.0f, dt); // Eases to hovered size

                if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                    ButtonActions[i](); // Execute button action when clicked
                }
            }
        }

        // Check right column
        for (int i = 0; i < columnButtons; i++) {
            float y = startY - (i * verticalSpacing);

            // Check if mouse clicks right column button
            float normRightX = rightStartX / 350.0f - 0.2f;
            float normRightY = y / 300.0f;

            // Get the width and height of the button text already normalized
            f32 righttextWidth = 0, righttextHeight = 0;
            AEGfxGetPrintSize(UIManager::GetFontId(), rightColumnLabels[i], 1.0f, &righttextWidth, &righttextHeight);

            if (normX >= normRightX && normX <= normRightX + righttextWidth &&
                normY >= normRightY && normY <= normRightY + righttextHeight) {
                buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] = HOVER_COLOR; // Change to yellow when hovered
                buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] = Lerp(buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons], HOVER_SCALE, 4.0f, dt);


                if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                    ButtonActions[i + columnButtons](); // Execute button action when clicked
                }
            }
        }

        // Center QUIT GAME button between the two columns
        float quitY = startY - (columnButtons * verticalSpacing) - 30.0f;
        float quitX = leftStartX + (columnSpacing / 2); // Center between columns
        float normQuitX = quitX / 350.0f - 0.2f;
        float normQuitY = quitY / 400.0f;

        f32 quittextWidth = 0, quittextHeight = 0;
        AEGfxGetPrintSize(UIManager::GetFontId(), "QUIT GAME", 1.0f, &quittextWidth, &quittextHeight);

        if (normX >= normQuitX && normX <= normQuitX + quittextWidth &&
            normY >= normQuitY && normY <= normQuitY + quittextHeight) {
            buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] = HOVER_COLOR; // Change to yellow when hovered
            buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] = Lerp(buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2], HOVER_SCALE, 4.0f, dt);

            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                ButtonActions[columnButtons * 2](); // Execute Quit Game action when clicked
            }
        }
        else {
            buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] = 0.5f; // Reset to white when not hovered
        }
    }
    if (ShowQuitConfirmation) {
        PopUpQuit_Update();
    }
}


void Menu_Draw() {
	AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);

	// Render stars first
	RenderStars(starMesh, supernovaStarMesh);

	// Draw earth sprite
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(earthSpriteTexture, 0, 0);

	// Keep earth sprite on the left side
    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, 820.0f, 820.0f);

    AEMtx33 rotation = { 0 };
    AEMtx33Rot(&rotation, earthRotation);

    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, -345.0f, -45.0f);

    AEMtx33 tempTransform = { 0 };
    AEMtx33 transform = { 0 };

    AEMtx33Concat(&tempTransform, &rotation, &scale);
    AEMtx33Concat(&transform, &translate, &tempTransform);

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(earthSpriteMesh, AE_GFX_MDM_TRIANGLES);

    MainMenuObjects::Draw(); // Draw the ship on top of other UI elements

    // Draw title using the UIManager
    uiManager->DrawTitle("A S T R A L  S I E G E");

    // Reset render mode for buttons
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(0.5f);

    // Two-column menu layout on the right side of earth
    const char* leftColumnLabels[] = { "START GAME", "CREDITS" };
    const char* rightColumnLabels[] = { "CONTROLS", "OPTIONS" };
    const int columnButtons = 2;

    // Position both columns on the right side of earth
    float columnSpacing = 150.0f;  // Reduced spacing between columns
    float leftStartX = 125.0f;    // Moved first column more to the right
    float rightStartX = leftStartX + columnSpacing;  // Second column position adjusts automatically
    float startY = 60.0f;
    float verticalSpacing = 90.0f;

    // Draw left column
    for (int i = 0; i < columnButtons; i++) {
        float y = startY - (i * verticalSpacing);

        AEMtx33 btnScale = { 0 };
        AEMtx33Scale(&btnScale, 200.0f, 50.0f);
        AEMtx33 btnTrans = { 0 };
        AEMtx33Trans(&btnTrans, leftStartX, y);
        AEMtx33 btnTransform = { 0 };
        AEMtx33Concat(&btnTransform, &btnTrans, &btnScale);

        AEGfxSetTransform(btnTransform.m);
        AEGfxMeshDraw(menuButtonMesh, AE_GFX_MDM_TRIANGLES);

        float normX = leftStartX / 400.0f;
        float normY = y / 300.0f;

        // Get text dimensions
        f32 textWidth = 0, textHeight = 0;
        AEGfxGetPrintSize(UIManager::GetFontId(), leftColumnLabels[i], 1.0f, &textWidth, &textHeight);

        // Calculate center position
        float centerX = normX - 0.2f + (textWidth * DEFAULT_SCALE * scaleFactor / 2.0f);
        float centerY = normY + (textHeight * DEFAULT_SCALE * scaleFactor / 2.0f);

        // Adjust position based on scale to keep center fixed
        float adjustedX = centerX - (textWidth * buttonScale[i] * scaleFactor / 2.0f);
        float adjustedY = centerY - (textHeight * buttonScale[i] * scaleFactor / 2.0f);

        AEGfxPrint(
            UIManager::GetFontId(),
            leftColumnLabels[i],
            adjustedX,
            adjustedY,
            buttonScale[i] * scaleFactor,
            buttonColor[i] == DEFAULT_COLOR ? WHITE_R : YELLOW_R,
            buttonColor[i] == DEFAULT_COLOR ? WHITE_G : YELLOW_G,
            buttonColor[i] == DEFAULT_COLOR ? WHITE_B : YELLOW_B,
            1.0f  // Alpha
        );
    }

    // Draw right column
    for (int i = 0; i < columnButtons; i++) {
        float y = startY - (i * verticalSpacing);

        AEMtx33 btnScale = { 0 };
        AEMtx33Scale(&btnScale, 200.0f, 50.0f);
        AEMtx33 btnTrans = { 0 };
        AEMtx33Trans(&btnTrans, rightStartX, y);
        AEMtx33 btnTransform = { 0 };
        AEMtx33Concat(&btnTransform, &btnTrans, &btnScale);

        AEGfxSetTransform(btnTransform.m);
        AEGfxMeshDraw(menuButtonMesh, AE_GFX_MDM_TRIANGLES);

        float normX = rightStartX / 350.0f;
        float normY = y / 300.0f;

        // Get text dimensions
        f32 textWidth = 0, textHeight = 0;
        AEGfxGetPrintSize(UIManager::GetFontId(), rightColumnLabels[i], 1.0f, &textWidth, &textHeight);

        // Calculate center position
        float centerX = normX - 0.2f + (textWidth * DEFAULT_SCALE * scaleFactor / 2.0f);
        float centerY = normY + (textHeight * DEFAULT_SCALE * scaleFactor / 2.0f);

        // Adjust position based on scale to keep center fixed
        float adjustedX = centerX - (textWidth * buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] * scaleFactor / 2.0f);
        float adjustedY = centerY - (textHeight * buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] * scaleFactor / 2.0f);

        AEGfxPrint(
            UIManager::GetFontId(),
            rightColumnLabels[i],
            adjustedX,
            adjustedY,
            buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] * scaleFactor,
            buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] == DEFAULT_COLOR ? WHITE_R : YELLOW_R,
            buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] == DEFAULT_COLOR ? WHITE_G : YELLOW_G,
            buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) + columnButtons] == DEFAULT_COLOR ? WHITE_B : YELLOW_B,
            1.0f  // Alpha
        );
    }

    // Center QUIT GAME button between the two columns
    float quitY = startY - (columnButtons * verticalSpacing) - 30.0f;
    float quitX = leftStartX + (columnSpacing / 2); // Center between columns

    AEMtx33 quitBtnScale = { 0 };
    AEMtx33Scale(&quitBtnScale, 200.0f, 50.0f);
    AEMtx33 quitBtnTrans = { 0 };
    AEMtx33Trans(&quitBtnTrans, quitX, quitY);
    AEMtx33 quitBtnTransform = { 0 };
    AEMtx33Concat(&quitBtnTransform, &quitBtnTrans, &quitBtnScale);

    AEGfxSetTransform(quitBtnTransform.m);
    AEGfxMeshDraw(menuButtonMesh, AE_GFX_MDM_TRIANGLES);

    float normQuitX = quitX / 350.0f;
    float normQuitY = quitY / 400.0f;

    // Get text dimensions
    f32 quitTextWidth = 0, quitTextHeight = 0;
    AEGfxGetPrintSize(UIManager::GetFontId(), "QUIT GAME", 1.0f, &quitTextWidth, &quitTextHeight);

    // Calculate center position
    float quitCenterX = normQuitX - 0.2f + (quitTextWidth * DEFAULT_SCALE * scaleFactor / 2.0f);
    float quitCenterY = normQuitY + (quitTextHeight * DEFAULT_SCALE * scaleFactor / 2.0f);

    // Adjust position based on scale to keep center fixed
    float adjustedQuitX = quitCenterX - (quitTextWidth * buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] * scaleFactor / 2.0f);
    float adjustedQuitY = quitCenterY - (quitTextHeight * buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] * scaleFactor / 2.0f);

    AEGfxPrint(
        UIManager::GetFontId(),
        "QUIT GAME",
        adjustedQuitX,
        adjustedQuitY,
        buttonScale[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] * scaleFactor,
        buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] == DEFAULT_COLOR ? WHITE_R : YELLOW_R,
        buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] == DEFAULT_COLOR ? WHITE_G : YELLOW_G,
        buttonColor[static_cast<std::vector<float, std::allocator<float>>::size_type>(columnButtons) * 2] == DEFAULT_COLOR ? WHITE_B : YELLOW_B,
        1.0f  // Alpha
    );

    if (ShowQuitConfirmation) {
        PopUpQuit_Draw();
    }
}

void Menu_Unload() {
	AEGfxMeshFree(menuButtonMesh);
    menuButtonMesh = nullptr;

	AEGfxMeshFree(earthSpriteMesh);
    earthSpriteMesh = nullptr;

	AEGfxTextureUnload(earthSpriteTexture);
    earthSpriteTexture = nullptr;
} 

void Menu_Free() {
    MeshManager::GetInstance().UnloadTexture();
    MeshManager::GetInstance().UnloadMesh();

    MainMenuObjects::Cleanup();

	// Free ship mesh
	AEGfxMeshFree(shipMesh);
	shipMesh = nullptr;

    AEGfxMeshFree(starMesh);
    starMesh = nullptr;

    AEGfxMeshFree(supernovaStarMesh);
    supernovaStarMesh = nullptr;

    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;
}
