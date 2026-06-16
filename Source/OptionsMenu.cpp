/******************************************************************************/
/**
 * @file        OptionsMenu.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (Primary) - 80%
 * @author      Chloe Lau Rey En (Secondary) - added other volume bar and linking to audio (20%)
 * @brief       Manages the options menu state.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"
#include "../Header/OptionsMenu.hpp"
#include "../Header/AudioManager.hpp"

// Forward declaration of structs
struct VolumeSlider;
struct VectorToggle;
struct UIColors;

// Forward declarations
void UpdateVolumeSlider(VolumeSlider& slider);
void UpdateSlider();
void DrawSlider(VolumeSlider& slider, float x, float y, const char* label);
void UpdateVectorEnable();
void DrawVectorEnable();

// Global variables
bool IsVectorEnabled = false;
MeshManager* meshManagers = nullptr;
static const float UI_HORIZONTAL_OFFSET = -20.0f;
static const float Screen_Width = 1600.0f;
static const float Screen_Height = 900.0f;

// Slider settings
struct VolumeSlider {
    float width;
    float height;
    float posX;
    float posY;
    float thumbWidth;
    float thumbHeight;
    float value;
    bool isDragging;
    bool isHovering;
    bool isSFX; // to check which grp to control

    // Colors
    float bgColor[3];   // Background color
    float fillColor[3]; // Fill color
    float thumbColor[3]; // Thumb color
};

// Vector toggle settings
struct VectorToggle {
    bool isEnabled;
    float buttonScales[2];
    float buttonColors[2];
    const char* buttonLabels[2];
};

// UI Colors
struct UIColors {
    float white[3];
    float yellow[3];
    float defaultColor;
    float hoverColor;
    float defaultScale;
    float hoverScale;
};

// Initialize structures
VolumeSlider gMusicSlider;
VolumeSlider gSFXSlider;
VolumeSlider gMasterSlider;
VectorToggle gVectorToggle;
UIColors gColors;

static void InitSliders() {
    // Common properties for both sliders
    float sliderWidth = 200.0f;
    float sliderHeight = 20.0f;
    float thumbWidth = 20.0f;
    float thumbHeight = 40.0f;

    gMusicSlider.width = sliderWidth;
    gMusicSlider.height = sliderHeight;
    gMusicSlider.posX = -sliderWidth / 2;
    gMusicSlider.posY = 70.0f;
    gMusicSlider.thumbWidth = thumbWidth;
    gMusicSlider.thumbHeight = thumbHeight;
    gMusicSlider.value = 1.0f;
    gMusicSlider.isDragging = false;
    gMusicSlider.isHovering = false;
    gMusicSlider.isSFX = false;

    // Set colors
    gMusicSlider.bgColor[0] = 0.2f;
    gMusicSlider.bgColor[1] = 0.2f;
    gMusicSlider.bgColor[2] = 0.2f;

    gMusicSlider.fillColor[0] = 0.8f;
    gMusicSlider.fillColor[1] = 0.2f;
    gMusicSlider.fillColor[2] = 0.2f;

    gMusicSlider.thumbColor[0] = 1.0f;
    gMusicSlider.thumbColor[1] = 1.0f;
    gMusicSlider.thumbColor[2] = 1.0f;

    // SFX volume slider
    gSFXSlider.width = sliderWidth;
    gSFXSlider.height = sliderHeight;
    gSFXSlider.posX = -sliderWidth / 2;
    gSFXSlider.posY = -60.0f; // Position below master slider
    gSFXSlider.thumbWidth = thumbWidth;
    gSFXSlider.thumbHeight = thumbHeight;
    gSFXSlider.value = 1.0f;
    gSFXSlider.isDragging = false;
    gSFXSlider.isHovering = false;
    gSFXSlider.isSFX = true;

    // Set colors for SFX slider
    gSFXSlider.bgColor[0] = 0.2f;
    gSFXSlider.bgColor[1] = 0.2f;
    gSFXSlider.bgColor[2] = 0.2f;

    gSFXSlider.fillColor[0] = 0.2f;
    gSFXSlider.fillColor[1] = 0.2f;
    gSFXSlider.fillColor[2] = 0.8f; // Different color to distinguish

    gSFXSlider.thumbColor[0] = 1.0f;
    gSFXSlider.thumbColor[1] = 1.0f;
    gSFXSlider.thumbColor[2] = 1.0f;

    gMasterSlider.width = sliderWidth;
    gMasterSlider.height = sliderHeight;
    gMasterSlider.posX = -sliderWidth / 2;
    gMasterSlider.posY = 200.0f; // Position above other sliders
    gMasterSlider.thumbWidth = thumbWidth;
    gMasterSlider.thumbHeight = thumbHeight;
    gMasterSlider.value = 1.0f;
    gMasterSlider.isDragging = false;
    gMasterSlider.isHovering = false;
    gMasterSlider.isSFX = false;  // Not used for master volume logic

    // Master slider
    gMasterSlider.bgColor[0] = 0.2f;
    gMasterSlider.bgColor[1] = 0.2f;
    gMasterSlider.bgColor[2] = 0.2f;

    gMasterSlider.fillColor[0] = 0.2f;
    gMasterSlider.fillColor[1] = 0.8f;  // Green tint for master
    gMasterSlider.fillColor[2] = 0.2f;

    gMasterSlider.thumbColor[0] = 1.0f;
    gMasterSlider.thumbColor[1] = 1.0f;
    gMasterSlider.thumbColor[2] = 1.0f;
}

static void InitVectorToggle() {
    gVectorToggle.isEnabled = IsVectorEnabled; // Default to NO
    gVectorToggle.buttonLabels[0] = "YES";
    gVectorToggle.buttonLabels[1] = "NO";

    for (int i = 0; i < 2; i++) {
        gVectorToggle.buttonScales[i] = gColors.defaultScale;
        gVectorToggle.buttonColors[i] = gColors.defaultColor;
    }
}

static void InitColors() {
    gColors.white[0] = 1.0f;
    gColors.white[1] = 1.0f;
    gColors.white[2] = 1.0f;

    gColors.yellow[0] = 1.0f;
    gColors.yellow[1] = 1.0f;
    gColors.yellow[2] = 0.0f;

    gColors.defaultColor = 0.5f;
    gColors.hoverColor = 1.0f;
    gColors.defaultScale = 1.0f;
    gColors.hoverScale = 1.3f;
}

void Options_Load() {
    MeshManager::GetInstance().LoadMesh();
    MeshManager::GetInstance().LoadTexture();

    uiManager = new UIManager();
    uiManager->InitializeFonts();

    // Initialize structs
    InitColors();
    InitSliders();
    InitVectorToggle();

    gMasterSlider.value = AudioManager::GetInstance().GetMasterVolume();
    gMusicSlider.value = AudioManager::GetInstance().GetBGMVolume();
    gSFXSlider.value = AudioManager::GetInstance().GetSFXVolume();
}

void Options_Initialize() {
    // Create star meshes first
    meshManagers = &MeshManager::GetInstance();
    starMesh = createCircleMesh(0xFFFFFFFF);
    supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);

    InitializeStars(); // Initialize the star field
    ShowConfirmation = false;
}

void Options_Update() {
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
    UpdateStars(dt); // Update star positions

    UpdateSlider();
    UpdateVectorEnable();
    BackButton_Update();
    PopUp_Update();
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        ShowConfirmation = !ShowConfirmation;
    }
}

void Options_Draw() {
    AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);
    RenderStars(starMesh, supernovaStarMesh);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.0f);

    uiManager->DrawTitle("Options");

    // Update slider positions for hit detection (these are the true center positions)
    gMasterSlider.posX = 0.0f + UI_HORIZONTAL_OFFSET;
    gMasterSlider.posY = 200.0f;

    gMusicSlider.posX = 0.0f + UI_HORIZONTAL_OFFSET;
    gMusicSlider.posY = 70.0f;

    gSFXSlider.posX = 0.0f + UI_HORIZONTAL_OFFSET;
    gSFXSlider.posY = -60.0f;

    // Draw sliders with labels
    DrawSlider(gMasterSlider, 0.0f, 200.0f, "Master Volume");
    DrawSlider(gMusicSlider, 0.0f, 70.0f, "BGM Volume");
    DrawSlider(gSFXSlider, 0.0f, -60.0f, "Sound Effects Volume");

    DrawVectorEnable();
    BackButton_Draw();
}

void Options_Free() {
    AEGfxMeshFree(starMesh);
    AEGfxMeshFree(supernovaStarMesh);
}

void Options_Unload() {
    // Free UI resources
    uiManager->DestroyFonts();
    delete uiManager;
    uiManager = nullptr;

    meshManagers->UnloadTexture();
    meshManagers->UnloadMesh();
}

// Helper function for smooth transitions
static float LerpVal(float a, float b, float t, float dt) {
    // Simple linear interpolation
    return a + (b - a) * t * dt;
}

void UpdateSlider() {
    UpdateVolumeSlider(gMasterSlider);
    UpdateVolumeSlider(gMusicSlider);
    UpdateVolumeSlider(gSFXSlider);
}

void DrawSlider(VolumeSlider& slider, float x, float y, const char* label) {
    // Store the actual draw position but don't modify slider.posX
    float drawPosX = x + UI_HORIZONTAL_OFFSET;
    float drawPosY = y;

    AEMtx33 scale, trans, transform;

    if (label && *label) {  // Check if label exists and isn't empty
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), label, 1.0f, &textWidth, &textHeight);

        // Calculate a proportional offset based on slider dimensions
        float verticalOffset = slider.thumbHeight * 0.8f;  // Proportional to thumb height

        float textLeftOffset = -0.02f; // Adjust this value as needed to shift text left

        // Apply this offset to the text position only
        float startX = -textWidth / 2.0f + textLeftOffset;
        float startY = (y + verticalOffset) / (Screen_Height / 2);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        AEGfxPrint(
            UIManager::GetFontId(),
            label,
            startX,
            startY,
            1.0f,
            gColors.white[0],
            gColors.white[1],
            gColors.white[2],
            1.0f
        );
    }

    // Draw the slider background
    AEGfxSetColorToMultiply(slider.bgColor[0], slider.bgColor[1], slider.bgColor[2], 1.0f);
    AEMtx33Scale(&scale, slider.width, slider.height);
    AEMtx33Trans(&trans, drawPosX, drawPosY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    // Draw the filled part of the slider
    AEGfxSetColorToMultiply(slider.fillColor[0], slider.fillColor[1], slider.fillColor[2], 1.0f);
    AEMtx33Scale(&scale, slider.width * slider.value, slider.height);
    AEMtx33Trans(&trans, drawPosX - (slider.width / 2) + (slider.width * slider.value / 2), drawPosY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    // Draw the slider thumb
    float thumbX = drawPosX - (slider.width / 2) + (slider.width * slider.value);
    AEGfxSetColorToMultiply(slider.thumbColor[0], slider.thumbColor[1], slider.thumbColor[2], 1.0f);
    AEMtx33Scale(&scale, slider.thumbWidth, slider.thumbHeight);
    AEMtx33Trans(&trans, thumbX, drawPosY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    // Reset color
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

void UpdateVectorEnable() {
    auto cursor = UIManager::GetNormalizedCursor();

    const int buttonCount = 2;
    float horizontalSpacing = 180.0f / Screen_Width;

    // Calculate total width of buttons
    float totalTextWidth = 0.0f;
    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), gVectorToggle.buttonLabels[i], 1.0f, &textWidth, &textHeight);
        totalTextWidth += textWidth;
    }

    float startX = -(totalTextWidth + horizontalSpacing) / 2.0f - 20.0f / Screen_Width;

    // Process each button
    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), gVectorToggle.buttonLabels[i], 1.0f, &textWidth, &textHeight);

        // Position the button in the options menu
        float offsetY = -450.0f / Screen_Height;
        float y = offsetY;

        // Calculate button hitbox
        float leftX = startX;
        float rightX = startX + textWidth * gVectorToggle.buttonScales[i];
        float topY = y;
        float bottomY = y + textHeight * gVectorToggle.buttonScales[i];

        // Check for hover and click
        bool isHovered = (cursor.first >= leftX && cursor.first <= rightX &&
            cursor.second >= topY && cursor.second <= bottomY);

        if (isHovered) {
            // Smooth transition to hover state
            gVectorToggle.buttonColors[i] = LerpVal(gVectorToggle.buttonColors[i], gColors.hoverColor, 10.0f, 0.016f);
            gVectorToggle.buttonScales[i] = LerpVal(gVectorToggle.buttonScales[i], gColors.hoverScale, 10.0f, 0.016f);

            // Handle click
            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                gVectorToggle.isEnabled = (i == 0);
                IsVectorEnabled = gVectorToggle.isEnabled;
            }
        }
        else {
            // Smooth transition back to default when not hovered
            gVectorToggle.buttonColors[i] = LerpVal(gVectorToggle.buttonColors[i], gColors.defaultColor, 10.0f, 0.016f);
            gVectorToggle.buttonScales[i] = LerpVal(gVectorToggle.buttonScales[i], gColors.defaultScale, 10.0f, 0.016f);
        }

        startX += textWidth * gVectorToggle.buttonScales[i] + horizontalSpacing;
    }
}

void DrawVectorEnable() {
    float textWidth, textHeight;

    // Draw "Enable Arrows?" text
    const char* label = "Enable Arrows?";
    AEGfxGetPrintSize(UIManager::GetFontId(), label, 1.0f, &textWidth, &textHeight);

    float centerX = -textWidth / 2.0f - 20.0f / Screen_Width;
    float textY = -300.0f / Screen_Height;

    AEGfxPrint(UIManager::GetFontId(), label, centerX, textY, 1.0f,
        gColors.white[0], gColors.white[1], gColors.white[2], 1.0f);

    // Draw Yes/No buttons
    const int buttonCount = 2;
    float horizontalSpacing = 180.0f / Screen_Width;

    // Calculate total width for centering
    float totalTextWidth = 0.0f;
    for (int i = 0; i < buttonCount; i++) {
        AEGfxGetPrintSize(UIManager::GetFontId(), gVectorToggle.buttonLabels[i], 1.0f, &textWidth, &textHeight);
        totalTextWidth += textWidth;
    }

    float startX = -(totalTextWidth + horizontalSpacing) / 2.0f - 20.0f / Screen_Width;

    // Draw each button
    for (int i = 0; i < buttonCount; i++) {
        AEGfxGetPrintSize(UIManager::GetFontId(), gVectorToggle.buttonLabels[i], 1.0f, &textWidth, &textHeight);

        float offsetY = -450.0f / Screen_Height;
        float y = offsetY;

        // Determine button color based on selection and hover state
        bool isSelected = (i == 0 && gVectorToggle.isEnabled) || (i == 1 && !gVectorToggle.isEnabled);

        // Choose color based on selection
        float r, g, b;
        if (isSelected) {
            // Selected option is yellow
            r = gColors.yellow[0];
            g = gColors.yellow[1];
            b = gColors.yellow[2];
        }
        else {
            // Non-selected option is white
            r = gColors.white[0];
            g = gColors.white[1];
            b = gColors.white[2];
        }

        // Draw the button with correct color
        AEGfxPrint(
            UIManager::GetFontId(),
            gVectorToggle.buttonLabels[i],
            startX,
            y,
            gVectorToggle.buttonScales[i],
            r, g, b,
            1.0f
        );

        startX += textWidth + horizontalSpacing;
    }

    // Reset color
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

void UpdateVolumeSlider(VolumeSlider& slider) {
    auto cursor = UIManager::GetNormalizedCursor();

    // Use slider's stored position (now correctly includes the offset)
    float normSliderStartX = (slider.posX - slider.width / 2) / (Screen_Width / 2);
    float normSliderWidth = slider.width / (Screen_Width / 2);

    // Calculate the position of the thumb in normalized coordinates
    float normThumbX = normSliderStartX + (normSliderWidth * slider.value);

    // Calculate the hit area for the thumb in normalized coordinates
    float normThumbWidth = (slider.thumbWidth / Screen_Width) * 2.0f;
    float normThumbHeight = (slider.thumbHeight / Screen_Height) * 2.0f;

    float minThumbX = normThumbX - normThumbWidth / 2;
    float maxThumbX = normThumbX + normThumbWidth / 2;
    float minThumbY = (slider.posY / (Screen_Height / 2)) - normThumbHeight / 2;
    float maxThumbY = (slider.posY / (Screen_Height / 2)) + normThumbHeight / 2;

    // Check if mouse is hovering over the thumb
    slider.isHovering = (cursor.first >= minThumbX && cursor.first <= maxThumbX &&
        cursor.second >= minThumbY && cursor.second <= maxThumbY);

    // Start dragging when clicking on the thumb
    if (slider.isHovering && AEInputCheckTriggered(AEVK_LBUTTON)) {
        slider.isDragging = true;
    }

    // Allow clicking anywhere on the slider track
    if (!slider.isHovering && AEInputCheckTriggered(AEVK_LBUTTON)) {
        // Check if the click is on the slider track
        float minTrackX = normSliderStartX;
        float maxTrackX = normSliderStartX + normSliderWidth;
        float minTrackY = (slider.posY / (Screen_Height / 2)) - (slider.height / Screen_Height);
        float maxTrackY = (slider.posY / (Screen_Height / 2)) + (slider.height / Screen_Height);

        if (cursor.first >= minTrackX && cursor.first <= maxTrackX &&
            cursor.second >= minTrackY && cursor.second <= maxTrackY) {
            // Set the slider value based on click position
            slider.value = (cursor.first - normSliderStartX) / normSliderWidth;
            slider.value = fmaxf(0.0f, fminf(slider.value, 1.0f)); // Clamp value

            // Apply the volume change based on slider type
            if (&slider == &gMasterSlider) {
                AudioManager::GetInstance().SetMasterVolume(slider.value);
            }
            else if (slider.isSFX) {
                AudioManager::GetInstance().SetGroupVolume(true, slider.value); // SFX
            }
            else {
                AudioManager::GetInstance().SetGroupVolume(false, slider.value); // BG Music
            }

            // Start dragging from this position
            slider.isDragging = true;
        }
    }

    // Handle dragging
    if (slider.isDragging) {
        if (AEInputCheckReleased(AEVK_LBUTTON)) {
            slider.isDragging = false;
        }
        else {
            // Update slider value based on mouse position
            slider.value = (cursor.first - normSliderStartX) / normSliderWidth;
            slider.value = fmaxf(0.0f, fminf(slider.value, 1.0f)); // Clamp value

            // Apply the volume change based on slider type
            if (&slider == &gMasterSlider) {
                AudioManager::GetInstance().SetMasterVolume(slider.value);
            }
            else if (slider.isSFX) {
                AudioManager::GetInstance().SetGroupVolume(true, slider.value); // SFX
            }
            else {
                AudioManager::GetInstance().SetGroupVolume(false, slider.value); // BG Music
            }
        }
    }
}