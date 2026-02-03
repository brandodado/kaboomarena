#include "Player.hpp"
#include "Environment.hpp"
#include "Collision.hpp"
#include "Hazards.hpp"
#include "Main.hpp"  // Access to g_dt and gGameStateNext
#include "Levels.hpp"

// --- PHYSICS CONSTANTS ---
#define PLAYER_MOVE_SPEED 300.0f
#define PLAYER_JUMP_FORCE 650.0f // High force helps reach far platforms
#define PLAYER_GRAVITY    -1500.0f
#define PLAYER_SIZE       50.0f

Player hamster;

void Player_Load(Player* pPlayer)
{
    // Create the "Player" square mesh
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

    pPlayer->pMesh = AEGfxMeshEnd();
}

void Player_Init(Player* pPlayer, float startX, float startY)
{
    pPlayer->position.x = startX;
    pPlayer->position.y = startY;
    pPlayer->velocity.x = 0.0f;
    pPlayer->velocity.y = 0.0f;
    pPlayer->isGrounded = 0;
    pPlayer->scale.x = PLAYER_SIZE;
    pPlayer->scale.y = PLAYER_SIZE;

    // --- NEW: Reset Jumps on Init ---
    pPlayer->jumpsRemaining = 2;
}

void Player_Update(Player* pPlayer)
{
    float dt = g_dt;

    // 1. Input & Movement
    if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
        pPlayer->velocity.x = -PLAYER_MOVE_SPEED;
    else if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
        pPlayer->velocity.x = PLAYER_MOVE_SPEED;
    else
        pPlayer->velocity.x = 0.0f;

    // --- FIX: MANUAL JUMP TRIGGER ---
    // We use a static variable to remember if space was held last frame.
    // This manually creates a "Trigger" that never fails.
    static bool wasSpaceHeld = false;
    bool isSpaceHeld = AEInputCheckCurr(AEVK_SPACE);

    // If Space is Down NOW, but was NOT down before -> That's a press!
    if (isSpaceHeld && !wasSpaceHeld)
    {
        if (pPlayer->jumpsRemaining > 0)
        {
            pPlayer->velocity.y = PLAYER_JUMP_FORCE;
            pPlayer->isGrounded = 0;
            pPlayer->jumpsRemaining--;
        }
    }

    // Update the memory for the next frame
    wasSpaceHeld = isSpaceHeld;
    // --------------------------------

    // Variable Jump Height (Cut jump short if key released)
    // We check if space is NOT held, but player is moving UP
    if (!isSpaceHeld && pPlayer->velocity.y > 0)
    {
        pPlayer->velocity.y *= 0.5f;
    }

    // 2. Apply Gravity
    pPlayer->velocity.y += PLAYER_GRAVITY * dt;

    // 3. Update Position
    pPlayer->position.x += pPlayer->velocity.x * dt;
    pPlayer->position.y += pPlayer->velocity.y * dt;

    // 4. PLATFORM COLLISION
    pPlayer->isGrounded = 0;

    // Create Player AABB
    AABB playerAABB;
    playerAABB.min.x = pPlayer->position.x - (PLAYER_SIZE / 2.0f);
    playerAABB.max.x = pPlayer->position.x + (PLAYER_SIZE / 2.0f);
    playerAABB.min.y = pPlayer->position.y - (PLAYER_SIZE / 2.0f);
    playerAABB.max.y = pPlayer->position.y + (PLAYER_SIZE / 2.0f);

    for (int i = 0; i < GET_PLATFORM_INSTANCE_NUM; ++i)
    {
        if (Platforms[i].Current_State == 0) continue;

        AABB platAABB;
        platAABB.min.x = Platforms[i].position.x - (Platforms[i].scale.x / 2.0f);
        platAABB.max.x = Platforms[i].position.x + (Platforms[i].scale.x / 2.0f);
        platAABB.min.y = Platforms[i].position.y - (Platforms[i].scale.y / 2.0f);
        platAABB.max.y = Platforms[i].position.y + (Platforms[i].scale.y / 2.0f);

        if (CollisionIntersection_RectRect(playerAABB, platAABB))
        {
            // Landing Logic (Must be falling AND above the platform)
            if (pPlayer->velocity.y <= 0 && pPlayer->position.y > Platforms[i].position.y)
            {
                float newY = Platforms[i].position.y + (Platforms[i].scale.y / 2.0f) + (PLAYER_SIZE / 2.0f);

                pPlayer->position.y = newY;
                pPlayer->velocity.y = 0;
                pPlayer->isGrounded = 1;
                pPlayer->jumpsRemaining = 2; // Reset jumps
            }
        }
    }

    // 5. Death Pit
    if (pPlayer->position.y < -400.0f) {
        nextState = GS_RESTART;
    }
}

void Player_Draw(Player* pPlayer)
{
    if (!pPlayer->pMesh) return;

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f); // Green

    AEMtx33 scale, trans, result;
    AEMtx33Scale(&scale, PLAYER_SIZE, PLAYER_SIZE);
    AEMtx33Trans(&trans, pPlayer->position.x, pPlayer->position.y);
    AEMtx33Concat(&result, &trans, &scale);

    AEGfxSetTransform(result.m);
    AEGfxMeshDraw(pPlayer->pMesh, AE_GFX_MDM_TRIANGLES);
}

void Player_Free(Player* pPlayer)
{
    if (pPlayer->pMesh)
    {
        AEGfxMeshFree(pPlayer->pMesh);
        pPlayer->pMesh = 0;
    }
}