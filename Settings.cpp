#include "AEEngine.h"
#include "Main.hpp"   // For GameState and currentState
#include "Utils.hpp"  // For DrawMesh, sqrMesh, PointInRect
#include <stdio.h>

// --- INTERNAL VARIABLES ---
static AEGfxVertexList* pMeshButton = 0; // Shape for the button
static AEVec2 backButtonPos;             // Position of Back Button
static float btnWidth = 200.0f;
static float btnHeight = 60.0f;

// Mock Setting: Sound Toggle
static bool isSoundOn = true;
static AEVec2 soundButtonPos;

void Settings_Load()
{
    // Reuse the square mesh creator from Utils.cpp
    // White color allows us to tint it later using AEGfxSetTintColor
    pMeshButton = sqrMesh(0xFFFFFFFF);
}

void Settings_Init()
{
    // 1. Setup Button Positions
    // Place Back Button at bottom-left
    backButtonPos.x = -300.0f;
    backButtonPos.y = -200.0f;

    // Place Sound Button in the middle
    soundButtonPos.x = 0.0f;
    soundButtonPos.y = 50.0f;

    AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f); // Dark Grey Background
}

void Settings_Update()
{
    // 1. Get Mouse Position (Matches your MainMenu logic)
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Convert Screen Space (Pixels) to World Space (Coordinates)
    // Assuming 800x600 window with 0,0 in the center
    AEVec2 mouseWorld;
    mouseWorld.x = (float)(mouseX - 400);      // Center X is 400
    mouseWorld.y = (float)((600 - mouseY) - 300); // Flip Y, Center Y is 300

    // 2. Button Logic: BACK
    bool hoverBack = PointInRect(mouseWorld, backButtonPos, btnWidth, btnHeight);

    // Check click (Left Mouse Button)
    if (AEInputCheckTriggered(AEVK_LBUTTON) && hoverBack)
    {
        currentState = GS_MENU; // Return to Main Menu
        printf("Settings: Returned to Menu\n");
    }

    // 3. Button Logic: SOUND TOGGLE
    bool hoverSound = PointInRect(mouseWorld, soundButtonPos, btnWidth, btnHeight);

    if (AEInputCheckTriggered(AEVK_LBUTTON) && hoverSound)
    {
        isSoundOn = !isSoundOn; // Toggle True/False
        printf("Settings: Sound is now %s\n", isSoundOn ? "ON" : "OFF");
    }

    // 4. Keyboard Shortcut (ESC to go back)
    if (AEInputCheckTriggered(AEVK_ESCAPE))
    {
        currentState = GS_MENU;
    }
}

void Settings_Draw()
{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    // --- DRAW BACK BUTTON ---
    // Highlight if mouse is over it (Logic inside Draw for simplicity or passed from Update)
    // For this simple example, we recalculate hover or just draw static colors.

    // Let's re-get mouse for purely visual tinting (or store 'hoverBack' in a static var in Update)
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);
    AEVec2 mouseWorld;
    mouseWorld.x = (float)(mouseX - 400);
    mouseWorld.y = (float)((600 - mouseY) - 300);

    // Color Logic: Red if hovered, Grey if not
    if (PointInRect(mouseWorld, backButtonPos, btnWidth, btnHeight))
        AEGfxSetBlendColor(1.0f, 0.0f, 0.0f, 1.0f); // Red (Hover)
    else
        AEGfxSetBlendColor(0.5f, 0.5f, 0.5f, 1.0f); // Grey (Idle)

    DrawMesh(pMeshButton, backButtonPos, btnWidth, btnHeight);

    // --- DRAW SOUND BUTTON ---
    // Green if ON, Red if OFF
    if (isSoundOn)
        AEGfxSetBlendColor(0.0f, 0.8f, 0.0f, 1.0f); // Green
    else
        AEGfxSetBlendColor(0.8f, 0.0f, 0.0f, 1.0f); // Red

    DrawMesh(pMeshButton, soundButtonPos, btnWidth, btnHeight);

    // (Optional) You can add Text here if you have a Font system loaded
    // AEGfxPrint(fontId, isSoundOn ? "Sound: ON" : "Sound: OFF", ...);
}

void Settings_Free()
{
    // Reset any logic flags if needed
}

void Settings_Unload()
{
    // Clean up mesh
    if (pMeshButton)
    {
        AEGfxMeshFree(pMeshButton);
        pMeshButton = 0;
    }
}