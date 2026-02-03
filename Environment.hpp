#pragma once // Easier than ifndef/define guards
#include "AEEngine.h"
#include "Utils.hpp" // Ensure this has your AABB or other utility defines if needed

// --- CHANGE 1: ALIGN STRUCT WITH LEVELS.CPP ---
// We renamed "GameObject" to "Platform" to match your usage, 
// and used AEVec2 so the collision math works.
typedef struct Platform
{
    // Physics / Transform Data
    AEVec2 position;     // Replaces float x, y
    AEVec2 scale;        // Replaces float scaleX, scaleY
    float rotation;

    // Logic Data
    int Current_State;   // 1 = Active/Visible, 0 = Inactive

    // Rendering Data
    AEGfxTexture* pTex;
    u32 color;           // Optional: If you aren't using textures yet
} Platform;

// --- CHANGE 2: DEFINE CONSTANTS ---
// This allows the loops in Levels.cpp to work
#define GET_PLATFORM_INSTANCE_NUM 20 

// --- CHANGE 3: GLOBAL ACCESS ---
// We use a raw array instead of std::vector to keep it simple for C-style loops
extern Platform Platforms[GET_PLATFORM_INSTANCE_NUM];
static AEGfxVertexList* pMeshPlatform = 0;
static AEGfxTexture* pTexPlatform = 0;
static AEGfxTexture* pTexBG = 0;

// Lifecycle Functions
void Environment_Load();
void Environment_Init();
void Environment_Update();
void Environment_Draw();
void Environment_Free();
void Environment_Unload();