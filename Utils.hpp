#pragma once // <--- CRITICAL: Prevents "Multiple Definition" errors

#include "AEEngine.h" // <--- CRITICAL: Defines AEVec2 and AEGfxVertexList
#include "Main.hpp"   // Defines GameState

extern float const gameDt;

// Mesh Helpers
AEGfxVertexList* sqrMesh(unsigned int colour);
void DrawMesh(AEGfxVertexList* mesh, AEVec2 position, float w, float h);

// Math/UI Helpers
bool PointInRect(AEVec2 p, AEVec2 rectCenter, float w, float h);

// State Helpers
void goToSettings(GameState curr);
void outOfSettings();