#include "Environment.hpp"
#include "AEFrameRateController.h"
#include "Main.hpp"
#include "Utils.hpp" // Required for goToSettings
#include <stdio.h> 
#include "Player.hpp"

extern Player hamster;

Platform Platforms[GET_PLATFORM_INSTANCE_NUM];
static AEGfxVertexList* pMeshBG = 0; // Separate mesh for backgrounds (usually bigger)

// --- TEXTURES ---
// RESTORED: Background Layers
static AEGfxTexture* pTexBG1 = 0;
static AEGfxTexture* pTexBG2 = 0;
static AEGfxTexture* pTexBG3 = 0;
static AEGfxTexture* pTexBG4 = 0;
//static AEGfxTexture* pTexGround = 0;

void Environment_Load()
{
    // 1. Create Platform Mesh (Square)
    AEGfxMeshStart();
    AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f, 0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, 0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    pMeshPlatform = AEGfxMeshEnd();

    // 2. Create Background Mesh (Same shape, but separate pointer for safety)
    // We reuse the same mesh for simplicity since both are squares.
    pMeshBG = pMeshPlatform;

    //pTexGround = AEGfxTextureLoad("Assets/ground_green.png");

    // 4. RESTORED: Load Background Layers
    pTexBG1 = AEGfxTextureLoad("Assets/bg_layer1.png");
    pTexBG2 = AEGfxTextureLoad("Assets/bg_layer2.png");
    pTexBG3 = AEGfxTextureLoad("Assets/bg_layer3.png");
    pTexBG4 = AEGfxTextureLoad("Assets/bg_layer4.png");

    if (!pTexBG1 || !pTexBG2) printf("WARNING: Some background layers failed to load.\n");
}

void Environment_Init(){}

void Environment_Update()
{
    goToSettings(currentState);
}

// Helper function to draw a full-screen background layer
void DrawLayer(AEGfxTexture* pTex, float x, float y) {
    if (!pTex) return;

    AEGfxTextureSet(pTex, 0, 0);
    AEMtx33 scale, trans, m;

    // Scale to match screen size
    AEMtx33Scale(&scale, 1600.0f, 900.0f);

    AEMtx33Trans(&trans, x, y);

    AEMtx33Concat(&m, &trans, &scale);

    AEGfxSetTransform(m.m);
    AEGfxMeshDraw(pMeshBG, AE_GFX_MDM_TRIANGLES);
}

void Environment_Draw()
{
    if (!pMeshPlatform) return;

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // --- DRAW BACKGROUND LAYERS ---
    // Because the Camera is locked to the player, drawing the background
    // at the player's position makes it look static

    //// Floor
    //Platforms[0].Current_State = 1;
    //Platforms[0].position.x = 0.0f;
    //Platforms[0].position.y = -450.0f;
    //Platforms[0].scale.x = 1800.0f;  
    //Platforms[0].scale.y = 80.0f;
    //Platforms[0].color = 0xFF888888;

    // Far back layer (Sky)
    DrawLayer(pTexBG1, hamster.position.x, hamster.position.y);

    // --- 2. DRAW PLATFORMS ---
    //AEGfxTextureSet(pTexPlatform, 0, 0);
    // Middle layers - Move slightly less than the player for "Parallax" depth
    // (hamster.x * 0.9 means it scrolls slowly)
    DrawLayer(pTexBG2, hamster.position.x * 0.95f, hamster.position.y * 0.95f);
    DrawLayer(pTexBG3, hamster.position.x * 0.90f, hamster.position.y * 0.90f);
    DrawLayer(pTexBG4, hamster.position.x * 0.85f, hamster.position.y * 0.85f);

    // --- DRAW PLATFORMS ---
    AEGfxTextureSet(pTexPlatform, 0, 0);
}

void Environment_Free() {
}

void Environment_Unload()
{
    if (pMeshPlatform) {
        AEGfxMeshFree(pMeshPlatform);
        pMeshPlatform = 0;
    }

    // Clean up textures
    //if (pTexGround) { AEGfxTextureUnload(pTexGround); pTexGround = 0; }
    if (pTexBG1) { AEGfxTextureUnload(pTexBG1); pTexBG1 = 0; }
    if (pTexBG2) { AEGfxTextureUnload(pTexBG2); pTexBG2 = 0; }
    if (pTexBG3) { AEGfxTextureUnload(pTexBG3); pTexBG3 = 0; }
    if (pTexBG4) { AEGfxTextureUnload(pTexBG4); pTexBG4 = 0; }
}