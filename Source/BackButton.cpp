/******************************************************************************/
/**
 * @file        BackButton.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Implementation of the back screen which prompts the player
 *              if they want to head back to the main menu.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"


bool ShowConfirmation = false;
static const float Screen_Width = 1600.0f;
static const float Screen_Height = 900.0f;

// Back Button
static float Button_ColorMultiplier = 1.0f;
static bool IsHovered = false;
static bool IsPressed = false;

static const float Button_Width = 64.0f;
static const float Button_Height = 64.0f;
static float ButtonX = -Screen_Width / 2 + 60.0f;
static float ButtonY = Screen_Height / 2 - 60.0f; 

// Pop-up
static const float PopUp_Width = 400.0f;
static const float PopUp_Height = 250.0f;
static const float PopUpX = Screen_Width / 2;
static const float PopUpY = Screen_Height / 2;

// Cancel Button
static float Cancel_ColorMultiplier = 1.0f;
static bool IsHoveredCancel = false;
static bool IsCancelPressed = false;

static const float Cancel_Width = 40.0f;
static const float Cancel_Height = 40.0f;
float CancelX = (PopUp_Width / 2) - (Cancel_Width / 2) - 5.0f;
float CancelY = (PopUp_Height / 2) - (Cancel_Height / 2) - 5.0f;

// Yes, No Button Colour Change
static float buttonScale[2] = { 1.0f, 1.0f };
static float buttonColor[2] = { 0.5f, 0.5f };

const float DEFAULT_SCALE = 1.0f;
const float HOVER_SCALE = 1.3f;

const float DEFAULT_COLOR = 0.5f;
const float HOVER_COLOR = 1.0f;

const float WHITE_R = 1.0f;
const float WHITE_G = 1.0f;
const float WHITE_B = 1.0f;

const float YELLOW_R = 1.0f;
const float YELLOW_G = 1.0f;
const float YELLOW_B = 0.0f;


void BackButton_Update() {
    auto cursor = UIManager::GetNormalizedCursor();

    // Back Button
    float NormButtonWidth = (Button_Width / Screen_Width) * 2.0f;
    float NormButtonHeight = (Button_Height / Screen_Height) * 2.0f;
    float NormButtonX = ButtonX / (Screen_Width / 2);
    float NormButtonY = ButtonY / (Screen_Height / 2);

    float minX = NormButtonX - NormButtonWidth / 2;
    float maxX = NormButtonX + NormButtonWidth / 2;
    float minY = NormButtonY - NormButtonHeight / 2;
    float maxY = NormButtonY + NormButtonHeight / 2;

    // Check if the mouse is hovering over the button
    IsHovered = (cursor.first >= minX && cursor.first <= maxX &&
        cursor.second >= minY && cursor.second <= maxY) ? 1 : 0;

    if (IsHovered) {
        Button_ColorMultiplier = 0.8f;  // Darken on hover
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            IsPressed = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON)) {
            IsPressed = false;
            next = GS_MAINMENU;
        }
    }
    else {
        IsPressed = false;
        Button_ColorMultiplier = 1.0f;  // Reset to normal color
    }
}

void BackButton_Draw() {
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    AEMtx33 scale, trans, transform;
    AEMtx33Scale(&scale, Button_Width, Button_Height);
    AEMtx33Trans(&trans, ButtonX, ButtonY);
    AEMtx33Concat(&transform, &trans, &scale);

    // Change texture if pressed, otherwise use hover state
    if (IsPressed) {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON2), 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON1), 0, 0);
        AEGfxSetColorToMultiply(Button_ColorMultiplier, Button_ColorMultiplier, Button_ColorMultiplier, 1.0f);
    }

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTONBACK), 0, 0);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    if (ShowConfirmation) {
        PopUp_Draw();
    }
}



void PopUp_Update() {
    auto cursor = UIManager::GetNormalizedCursor();

    // Cancel Button
    float NormCancelWidth = (Cancel_Width / Screen_Width) * 2.0f;
    float NormCancelHeight = (Cancel_Height / Screen_Height) * 2.0f;
    float NormCancelX = CancelX / (Screen_Width / 2);
    float NormCancelY = CancelY / (Screen_Height / 2);

    float MinCancelX = NormCancelX - NormCancelWidth / 2;
    float MaxCancelX = NormCancelX + NormCancelWidth / 2;
    float MinCancelY = NormCancelY - NormCancelHeight / 2;
    float MaxCancelY = NormCancelY + NormCancelHeight / 2;

    IsHoveredCancel = (cursor.first >= MinCancelX && cursor.first <= MaxCancelX &&
        cursor.second >= MinCancelY && cursor.second <= MaxCancelY) ? 1 : 0;

    if (IsHoveredCancel) {
        Cancel_ColorMultiplier = 0.8f; // Darken on hover
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            IsCancelPressed = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON)) {
            IsCancelPressed = false;
            ShowConfirmation = false; // Close the confirmation box
        }
    }
    else {
        IsCancelPressed = false;
        Cancel_ColorMultiplier = 1.0f; // Reset to normal color
    }

    const char* buttonLabels[] = { "YES", "NO" };
    const int buttonCount = 2;

    float horizontalSpacing = 340.0f / Screen_Width;

    float totalTextWidth = 0.0f;
    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), buttonLabels[i], 1.0f, &textWidth, &textHeight);
        totalTextWidth += textWidth;
    }

    float startX = -(totalTextWidth + horizontalSpacing) / 2.0f;

    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), buttonLabels[i], 1.0f, &textWidth, &textHeight);

        float OffsetY = 150.0f / Screen_Height;
        float y = (-PopUp_Height / Screen_Height) + (textHeight / Screen_Height) + OffsetY;

        float leftX = startX;
        float rightX = startX + textWidth * buttonScale[i];
        float topY = y;
        float bottomY = y + textHeight * buttonScale[i];

        // Hover
        if (cursor.first >= leftX && cursor.first <= rightX &&
            cursor.second >= topY && cursor.second <= bottomY) {
            buttonColor[i] = Lerp(buttonColor[i], HOVER_COLOR, 10.0f, 0.016f);
            buttonScale[i] = Lerp(buttonScale[i], HOVER_SCALE, 10.0f, 0.016f);

            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                if (i == 0) {
                    next = GS_MAINMENU;
                    ShowConfirmation = false;
                }
                else {
                    ShowConfirmation = false;
                }
            }
        }
        else {
            // Reset to default when not hovered
            buttonColor[i] = DEFAULT_COLOR;
            buttonScale[i] = DEFAULT_SCALE;
        }

        startX += textWidth * buttonScale[i] + horizontalSpacing;
    }
}




void PopUp_Draw() {
    AEMtx33 scale = { 0 }, trans = { 0 }, transform = { 0 };
    AEGfxSetTransparency(1.0f);
    float NormPopUpX = (PopUpX - (Screen_Width / 2)) / (Screen_Width / 2);
    float NormPopUpY = (PopUpY - (Screen_Height / 2)) / (Screen_Height / 2);

    // Outline
    AEGfxSetColorToMultiply(0.584f, 0.769f, 0.831f, 1.0f);
    AEMtx33Scale(&scale, PopUp_Width + 10.0f, PopUp_Height + 10.0f);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // Inner Colour
    AEGfxSetColorToMultiply(0.396f, 0.522f, 0.675f, 1.0f);
    AEMtx33Scale(&scale, PopUp_Width, PopUp_Height);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // X button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransparency(1.0f);

    AEMtx33Scale(&scale, Cancel_Width, Cancel_Height);
    AEMtx33Trans(&trans, CancelX, CancelY);
    AEMtx33Concat(&transform, &trans, &scale);

    if (IsCancelPressed) {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON2), 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON1), 0, 0);
        AEGfxSetColorToMultiply(Cancel_ColorMultiplier, Cancel_ColorMultiplier, Cancel_ColorMultiplier, 1.0f);
    }

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTONCANCEL), 0, 0);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    // Save Game Text
    float offsetY = 300.0f / Screen_Height;
    float saveTextWidth, saveTextHeight;
    AEGfxGetPrintSize(UIManager::GetFontId(), "BACK TO MAIN MENU", 1.0f, &saveTextWidth, &saveTextHeight);
    float scaleSave = 1.3f;
    float centerX = (-saveTextWidth / 2.0f) * scaleSave;
    float saveGameY = (-PopUp_Height / Screen_Height) + (saveTextHeight / Screen_Height) + offsetY;

    AEGfxPrint(UIManager::GetFontId(), "BACK TO MAIN MENU", centerX, saveGameY, scaleSave, 1.0f, 1.0f, 1.0f, 1.0f);
    

    // Yes, No Button
    const char* buttonLabels[] = { "YES", "NO" };
    const int buttonCount = 2;

    float horizontalSpacing = 340.0f / Screen_Width;

    float totalTextWidth = 0.0f;
    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), buttonLabels[i], 1.0f, &textWidth, &textHeight);
        totalTextWidth += textWidth;
    }

    float startX = -(totalTextWidth + horizontalSpacing) / 2.0f;

    for (int i = 0; i < buttonCount; i++) {
        float textWidth, textHeight;
        AEGfxGetPrintSize(UIManager::GetFontId(), buttonLabels[i], 1.0f, &textWidth, &textHeight);

        float OffsetY = 150.0f / Screen_Height;
        float y = (-PopUp_Height / Screen_Height) + (textHeight / Screen_Height) + OffsetY;

        // Print button with dynamic color and size
        AEGfxPrint(
            UIManager::GetFontId(),
            buttonLabels[i],
            startX,
            y,
            buttonScale[i],
            buttonColor[i] == DEFAULT_COLOR ? WHITE_R : YELLOW_R,
            buttonColor[i] == DEFAULT_COLOR ? WHITE_G : YELLOW_G,
            buttonColor[i] == DEFAULT_COLOR ? WHITE_B : YELLOW_B,
            1.0f
        );

        startX += textWidth + horizontalSpacing;
    }

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}
