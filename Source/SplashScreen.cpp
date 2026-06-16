/******************************************************************************/
/**
 * @file        SplashScreen.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Manages the DigiPen Logo splash screen.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/SplashScreen.hpp"

// Splash Screen Variables
AEGfxTexture* splashLogo = nullptr;
AEGfxVertexList* splashMesh = nullptr;
float alpha = 0.0f;      // Transparency level for fade effect
float timer = 0.0f;      // Timer for animation
float fadeInTime = 2.0f; // Time for fade-in effect
float holdTime = 1.5f;   // Time to keep the logo fully visible
float fadeOutTime = 2.0f; // Time for fade-out effect

void Splash_Load() {
    // Load the splash screen logo texture
    splashLogo = AEGfxTextureLoad("Assets/DigiPen_CMYK_Red.png");

    // Create a quad mesh for rendering the texture
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // Bottom-left
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,  // Bottom-right
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f   // Top-left
    );
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,  // Bottom-right
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,  // Top-right
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f   // Top-left
    );
    splashMesh = AEGfxMeshEnd();
}

void Splash_Initialize() {
    alpha = 0.0f;
    timer = 0.0f;
}

void Splash_Update() {
    float deltaTime = (float)AEFrameRateControllerGetFrameTime();
    timer += deltaTime;

    if (timer < fadeInTime) {
        alpha = timer / fadeInTime; // Fade-in effect
    }
    else if (timer < fadeInTime + holdTime) {
        alpha = 1.0f; // Hold full visibility
    }
    else if (timer < fadeInTime + holdTime + fadeOutTime) {
        alpha = 1.0f - (timer - fadeInTime - holdTime) / fadeOutTime; // Fade-out effect
    }
    else {
        next = GS_MAINMENU; // Transition to the Main Menu
    }

    // Allow user to skip splash screen
    if (AEInputCheckTriggered(AEVK_RETURN) || AEInputCheckTriggered(AEVK_SPACE) ||
        AEInputCheckTriggered(AEVK_ESCAPE) ||
        AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RBUTTON)) {
        next = GS_MAINMENU; // Instantly skip to Main Menu
    }
}

void Splash_Draw() {

    float screenWidth = 1600.0f;
    float screenHeight = 900.0f;

    float logoWidth = screenWidth - 200.0f;
    float logoHeight = screenHeight - 500.0f;

    AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(alpha);
    AEGfxTextureSet(splashLogo, 0, 0);

    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, logoWidth, logoHeight);
    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, 0.0f, 0.0f);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &translate, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(splashMesh, AE_GFX_MDM_TRIANGLES);

}

void Splash_Free() {
    // No dynamic memory allocation, nothing to free here
    if (splashMesh) {
        AEGfxMeshFree(splashMesh);
        splashMesh = nullptr;
    }
}

void Splash_Unload() {
    if (splashLogo) {
        AEGfxTextureUnload(splashLogo);
        splashLogo = nullptr;
    }
}
