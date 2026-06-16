/******************************************************************************/
/**
 * @file        PauseButton.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary)
 * @brief       Pauses the game with options.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/game.hpp"

bool PauseShowConfirmation = false;
static const float Screen_Width = 1600.0f;
static const float Screen_Height = 900.0f;

// Pause Button
static float Pause_ColorMultiplier = 1.0f;
static bool IsHoveredPause = false;
static bool IsPressedPause = false;

static const float Pause_Width = 64.0f;
static const float Pause_Height = 64.0f;
static float PauseX = Screen_Width / 2 - 60.0f;
static float PauseY = Screen_Height / 2 - 60.0f;

// Pop-up Pause
static const float PopUpPause_Width = 250.0f;
static const float PopUpPause_Height = 250.0f;
static const float PopUpPauseX = Screen_Width / 2;
static const float PopUpPauseY = Screen_Height / 2;


// Cancel Button
static float CancelPause_ColorMultiplier = 1.0f;
static bool IsHoveredPauseCancel = false;
static bool IsCancelPausePressed = false;

static float CancelPause_Width = 40.0f;
static float CancelPause_Height = 40.0f;
static float CancelPauseX = (PopUpPause_Width / 2) - (CancelPause_Width / 2) - 5.0f;
static float CancelPauseY = (PopUpPause_Height / 2) - (CancelPause_Height / 2) - 5.0f;

// Restart Button
const float DEFAULT_RESTART_SCALE = 1.0f;
const float HOVER_RESTART_SCALE = 1.3f;

static const float RestartButtonWidth = 150.0f;
static const float RestartButtonHeight = 40.0f;

static float RestartButtonX = 0.0f;
static float RestartButtonY = 80.0f;

static bool IsHoveredRestart = false;
static bool IsPressedRestart = false;

// Constants for text colors
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

static int hoveredOption = -1;

// Pop-up Quit
static const float PopUpQuit_Width = 400.0f;
static const float PopUpQuit_Height = 250.0f;
static const float PopUpQuitX = Screen_Width / 2;
static const float PopUpQuitY = Screen_Height / 2;

// Cancel Quit Button
bool ShowQuitConfirmation = false;
static float CancelQuit_ColorMultiplier = 1.0f;
static bool IsHoveredCancelQuit = false;
static bool IsCancelQuitPressed = false;

static const float CancelQuit_Width = 40.0f;
static const float CancelQuit_Height = 40.0f;
float CancelQuitX = (PopUpQuit_Width / 2) - (CancelQuit_Width / 2) - 5.0f;
float CancelQuitY = (PopUpQuit_Height / 2) - (CancelQuit_Height / 2) - 5.0f;


void PauseButton_Update() {
    auto cursor = UIManager::GetNormalizedCursor();

    // Pause Button
    float NormPauseWidth = (Pause_Width / Screen_Width) * 2.0f;
    float NormPauseHeight = (Pause_Height / Screen_Height) * 2.0f;
    float NormPauseX = PauseX / (Screen_Width / 2);
    float NormPauseY = PauseY / (Screen_Height / 2);

    float MinPauseX = NormPauseX - NormPauseWidth / 2;
    float MaxPauseX = NormPauseX + NormPauseWidth / 2;
    float MinPauseY = NormPauseY - NormPauseHeight / 2;
    float MaxPauseY = NormPauseY + NormPauseHeight / 2;

    IsHoveredPause = (cursor.first >= MinPauseX && cursor.first <= MaxPauseX &&
        cursor.second >= MinPauseY && cursor.second <= MaxPauseY) ? 1 : 0;

    if (IsHoveredPause) {
        Pause_ColorMultiplier = 0.8f;
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            IsPressedPause = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON)) {
            IsPressedPause = false;
            PauseShowConfirmation = !PauseShowConfirmation;
            gamePaused = !gamePaused;
        }
    }
    else {
        IsPressedPause = false;
        Pause_ColorMultiplier = 1.0f;
    }

    // Cancel Button
    float normCancelWidth = (CancelPause_Width / Screen_Width) * 2.0f;
    float normCancelHeight = (CancelPause_Height / Screen_Height) * 2.0f;
    float normCancelX = CancelPauseX / (Screen_Width / 2);
    float normCancelY = CancelPauseY / (Screen_Height / 2);

    float minCancelX = normCancelX - normCancelWidth / 2;
    float maxCancelX = normCancelX + normCancelWidth / 2;
    float minCancelY = normCancelY - normCancelHeight / 2;
    float maxCancelY = normCancelY + normCancelHeight / 2;

    IsHoveredPauseCancel = (cursor.first >= minCancelX && cursor.first <= maxCancelX &&
        cursor.second >= minCancelY && cursor.second <= maxCancelY) ? 1 : 0;

    if (IsHoveredPauseCancel) {
        CancelPause_ColorMultiplier = 0.8f;
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            IsCancelPausePressed = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON)) {
            IsCancelPausePressed = false;
            PauseShowConfirmation = false; // Close the popup
            gamePaused = false;
        }
    }
    else {
        IsCancelPausePressed = false;
        CancelPause_ColorMultiplier = 1.0f;
    }
}

void PauseButton_Draw() {
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    AEMtx33 scalePause, transPause, transformPause;
    AEMtx33Scale(&scalePause, Pause_Width, Pause_Height);
    AEMtx33Trans(&transPause, PauseX, PauseY);
    AEMtx33Concat(&transformPause, &transPause, &scalePause);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransparency(1.0f);

    if (IsPressedPause) {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON2), 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON1), 0, 0);
        AEGfxSetColorToMultiply(Pause_ColorMultiplier, Pause_ColorMultiplier, Pause_ColorMultiplier, 1.0f);
    }

    AEGfxSetTransform(transformPause.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTONPAUSE), 0, 0);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    if (PauseShowConfirmation) {
        PopUpPause_Draw();
    }

    if (ShowQuitConfirmation) {
        PopUpQuit_Update();
    }
}

void PopUpPause_Draw() {
    AEMtx33 scale = { 0 }, trans = { 0 }, transform = { 0 };
    AEGfxSetTransparency(1.0f);
    float NormPopUpX = (PopUpPauseX - (Screen_Width / 2)) / (Screen_Width / 2);
    float NormPopUpY = (PopUpPauseY - (Screen_Height / 2)) / (Screen_Height / 2);

    // Outline
    AEGfxSetColorToMultiply(0.584f, 0.769f, 0.831f, 1.0f);
    AEMtx33Scale(&scale, PopUpPause_Width + 10.0f, PopUpPause_Height + 10.0f);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // Inner Colour
    AEGfxSetColorToMultiply(0.396f, 0.522f, 0.675f, 1.0f);
    AEMtx33Scale(&scale, PopUpPause_Width, PopUpPause_Height);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    //Cancel Button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransparency(1.0f);

    AEMtx33Scale(&scale, CancelPause_Width, CancelPause_Height);
    AEMtx33Trans(&trans, CancelPauseX, CancelPauseY);
    AEMtx33Concat(&transform, &trans, &scale);

    if (IsCancelPausePressed) {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON2), 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON1), 0, 0);
        AEGfxSetColorToMultiply(CancelPause_ColorMultiplier, CancelPause_ColorMultiplier, CancelPause_ColorMultiplier, 1.0f);
    }

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTONCANCEL), 0, 0);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    // Draw heading text for the popup
    float tW, tH;
    AEGfxGetPrintSize(UIManager::GetFontId(), "Game Paused", 1.0f, &tW, &tH);
    float tX = -tW / 2.0f;
    float tY = (PopUpPause_Height / Screen_Height) - (tH / Screen_Height) - 170.0f / Screen_Height;;
    AEGfxPrint(UIManager::GetFontId(), "Game Paused", tX, tY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    // Button Render
    const char* labels[] = { "Main Menu", "Tutorial", "Quit Game" };
    hoveredOption = -1;

    auto cursor = UIManager::GetNormalizedCursor();

    for (int i = 0; i < static_cast<int>(PauseOption::COUNT); ++i) {
        float textW, textH;
        AEGfxGetPrintSize(UIManager::GetFontId(), labels[i], 1.0f, &textW, &textH);

        float offsetY = (220.0f - i * 80.0f) / Screen_Height;
        float x = -textW / 2.0f;
        float y = (-PopUpPause_Height / Screen_Height) + (textH / Screen_Height) + offsetY;

        float r = WHITE_R, g = WHITE_G, b = WHITE_B;

        bool isHovered = cursor.first >= x && cursor.first <= x + textW && cursor.second >= y && cursor.second <= y + textH;
        if (isHovered) {
            hoveredOption = i;
            r = YELLOW_R; g = YELLOW_G; b = YELLOW_B;

            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                switch (static_cast<PauseOption>(i)) {
                case PauseOption::MAIN_MENU: 
                    next = GS_MAINMENU; 
                    break;
                case PauseOption::TUTORIAL: 
                    next = GS_CONTROLS; 
                    break;
                case PauseOption::QUIT_GAME:
                    ShowQuitConfirmation = true;
                    break;
                default: 
                    break;
                }
            }
        }

        AEGfxPrint(UIManager::GetFontId(), labels[i], x, y, 1.0f, r, g, b, 1.0f);
    }

    if (ShowQuitConfirmation) {
        PopUpQuit_Draw();
    }
}


void PopUpQuit_Update() {
    auto cursor = UIManager::GetNormalizedCursor();

    // Cancel Button
    float NormCancelWidth = (CancelQuit_Width / Screen_Width) * 2.0f;
    float NormCancelHeight = (CancelQuit_Height / Screen_Height) * 2.0f;
    float NormCancelX = CancelQuitX / (Screen_Width / 2);
    float NormCancelY = CancelQuitY / (Screen_Height / 2);

    float MinCancelX = NormCancelX - NormCancelWidth / 2;
    float MaxCancelX = NormCancelX + NormCancelWidth / 2;
    float MinCancelY = NormCancelY - NormCancelHeight / 2;
    float MaxCancelY = NormCancelY + NormCancelHeight / 2;

    IsHoveredCancelQuit = (cursor.first >= MinCancelX && cursor.first <= MaxCancelX &&
        cursor.second >= MinCancelY && cursor.second <= MaxCancelY) ? 1 : 0;

    if (IsHoveredCancelQuit) {
        CancelQuit_ColorMultiplier = 0.8f; // Darken on hover
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            IsCancelQuitPressed = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON)) {
            IsCancelQuitPressed = false;
            ShowQuitConfirmation = false;
        }
    }
    else {
        IsCancelQuitPressed = false;
        CancelQuit_ColorMultiplier = 1.0f; // Reset to normal color
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
        float y = (-PopUpQuit_Height / Screen_Height) + (textHeight / Screen_Height) + OffsetY;

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
                    ShowQuitConfirmation = false;
                    next = GS_QUIT;
                }
                else {
                    ShowQuitConfirmation = false;
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




void PopUpQuit_Draw() {
    AEMtx33 scale = { 0 }, trans = { 0 }, transform = { 0 };
    AEGfxSetTransparency(1.0f);
    float NormPopUpX = (PopUpQuitX - (Screen_Width / 2)) / (Screen_Width / 2);
    float NormPopUpY = (PopUpQuitY - (Screen_Height / 2)) / (Screen_Height / 2);

    // Outline
    AEGfxSetColorToMultiply(0.584f, 0.769f, 0.831f, 1.0f);
    AEMtx33Scale(&scale, PopUpQuit_Width + 10.0f, PopUpQuit_Height + 10.0f);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // Inner Colour
    AEGfxSetColorToMultiply(0.396f, 0.522f, 0.675f, 1.0f);
    AEMtx33Scale(&scale, PopUpQuit_Width, PopUpQuit_Height);
    AEMtx33Trans(&trans, NormPopUpX, NormPopUpY);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // X button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTransparency(1.0f);

    AEMtx33Scale(&scale, CancelQuit_Width, CancelQuit_Height);
    AEMtx33Trans(&trans, CancelQuitX, CancelQuitY);
    AEMtx33Concat(&transform, &trans, &scale);

    if (IsCancelQuitPressed) {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON2), 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTON1), 0, 0);
        AEGfxSetColorToMultiply(CancelQuit_ColorMultiplier, CancelQuit_ColorMultiplier, CancelQuit_ColorMultiplier, 1.0f);
    }

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    AEGfxTextureSet(MeshManager::GetInstance().GetTexture(TextureType::BUTTONCANCEL), 0, 0);
    AEGfxMeshDraw(MeshManager::GetInstance().GetMesh(MeshType::QUAD), AE_GFX_MDM_TRIANGLES);

    // Save Game Text
    float offsetY = 300.0f / Screen_Height;
    float saveTextWidth, saveTextHeight;
    AEGfxGetPrintSize(UIManager::GetFontId(), "QUIT GAME?", 1.0f, &saveTextWidth, &saveTextHeight);
    float scaleSave = 1.3f;
    float centerX = (-saveTextWidth / 2.0f) * scaleSave;
    float saveGameY = (-PopUpQuit_Height / Screen_Height) + (saveTextHeight / Screen_Height) + offsetY;

    AEGfxPrint(UIManager::GetFontId(), "QUIT GAME?", centerX, saveGameY, scaleSave, 1.0f, 1.0f, 1.0f, 1.0f);


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
        float y = (-PopUpQuit_Height / Screen_Height) + (textHeight / Screen_Height) + OffsetY;

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
