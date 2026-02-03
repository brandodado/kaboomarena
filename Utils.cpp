#include "Main.hpp" 
#include <crtdbg.h> 
#include <math.h>
#include <stdio.h> // <--- CHANGE 1: Added for printf
#include "AEEngine.h"
#include "AEInput.h"
#include "AEMath.h"

// Global Delta Time Definition
float const gameDt = 1.f / 60.f;

// --- MESH CREATION ---
AEGfxVertexList* sqrMesh(unsigned int colour) {
    AEGfxMeshStart();
    // Triangle 1
    AEGfxTriAdd(
        -0.5f, -0.5f, colour, 0.0f, 1.0f,
        0.5f, -0.5f, colour, 1.0f, 1.0f,
        -0.5f, 0.5f, colour, 0.0f, 0.0f
    );
    // Triangle 2
    AEGfxTriAdd(
        0.5f, -0.5f, colour, 1.0f, 1.0f,
        0.5f, 0.5f, colour, 1.0f, 0.0f,
        -0.5f, 0.5f, colour, 0.0f, 0.0f
    );
    return AEGfxMeshEnd();
}

// --- DRAW MESH ---
void DrawMesh(AEGfxVertexList* mesh, AEVec2 position, float w, float h) {
    if (!mesh) return;

    AEMtx33 S, T, M;
    AEMtx33Scale(&S, w, h);
    AEMtx33Trans(&T, position.x, position.y);
    AEMtx33Concat(&M, &T, &S); // Scale then Translate

    // CHANGE 2: Pass the address of the Matrix struct (&M), not the array (M.m)
    AEGfxSetTransform(M.m);

    AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

// --- UI HELPER ---
bool PointInRect(AEVec2 p, AEVec2 rectCenter, float w, float h) {
    return (p.x > rectCenter.x - w * 0.5f && p.x < rectCenter.x + w * 0.5f
        && p.y > rectCenter.y - h * 0.5f && p.y < rectCenter.y + h * 0.5f);
}

// --- SETTINGS STATE LOGIC ---

// Define the global variable declared in Utils.hpp
//static bool isEscPressed = false; // "Debounce" variable

void goToSettings(GameState curr) {
    bool keyIsDown = AEInputCheckTriggered(AEVK_ESCAPE);

    if (keyIsDown && /*!isEscPressed &&*/ (curr != GS_SETTINGS)) {
        prevState = curr;
        nextState = GS_MENU;
        printf("Entered Settings\n");
        //isEscPressed = true;
    }

   // if (!keyIsDown) isEscPressed = false;
}

void outOfSettings() {
    bool keyIsDown = AEInputCheckTriggered(AEVK_ESCAPE);

    if (keyIsDown && /*!isEscPressed &&*/ (currentState == GS_MENU)) {
        nextState = prevState;
        printf("Exited Settings\n");
        //isEscPressed = true;
    }

    //if (!keyIsDown) isEscPressed = false;
}