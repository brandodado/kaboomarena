#include "Hazards.hpp"
#include <stdlib.h> // For rand()

static std::vector<Meteor> meteorList;
static AEGfxVertexList* pMeshMeteor = nullptr; // Internal mesh
static float spawnTimer = 0.0f;
static float SPAWN_RATE = 0.5f; // Spawns every 1 second

void Hazard_Load() {
    // 1. Create a simple Square Mesh for the Meteor
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    pMeshMeteor = AEGfxMeshEnd();
}

void Hazard_Init() {
    meteorList.clear();
    spawnTimer = 0.0f;
}

void Hazard_Update(float dt) {
    // 1. Spawning Logic
    spawnTimer -= dt;
    if (spawnTimer <= 0.0f) {
        Meteor m;
        // Random X: -400 to 400 (Screen width 800)
        m.x = (float)((rand() % 1600) - 800);
        m.y = 550.0f;       // Start just above screen
        m.velocityY = -400.0f; // Fall speed
        m.size = 80.0f;
        m.active = true;
        meteorList.push_back(m);

        spawnTimer = SPAWN_RATE; // Reset timer
    }

    // 2. Movement Logic
    for (int i = 0; i < (int)meteorList.size(); ++i) {
        if (!meteorList[i].active) continue;

        meteorList[i].y += meteorList[i].velocityY * dt;

        // Despawn if below screen (Y < -350)
        if (meteorList[i].y < -350.0f) {
            meteorList[i].active = false;
        }
    }
}

void Hazard_Draw() {
    if (!pMeshMeteor) return;

    // Set Render Mode (Color Mode usually, or Texture if you load one)
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    // Make Meteors RED
    AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < (int)meteorList.size(); ++i) {
        if (!meteorList[i].active) continue;

        // Matrix Math
        AEMtx33 scale, trans, result;
        AEMtx33Scale(&scale, meteorList[i].size, meteorList[i].size);
        AEMtx33Trans(&trans, meteorList[i].x, meteorList[i].y);
        AEMtx33Concat(&result, &trans, &scale);

        AEGfxSetTransform(result.m);
        AEGfxMeshDraw(pMeshMeteor, AE_GFX_MDM_TRIANGLES);
    }
}

void Hazard_Unload() {
    if (pMeshMeteor) {
        AEGfxMeshFree(pMeshMeteor);
        pMeshMeteor = nullptr;
    }
    meteorList.clear();
}

std::vector<Meteor>& GetMeteors() {
    return meteorList;
}