#include "Levels.hpp"
#include "Main.hpp"
#include "Collision.hpp"
#include "Player.hpp"
#include "Hazards.hpp"
#include "Environment.hpp"

// Global instances
extern Player hamster;

// External reference to the mesh variable (Assuming it is defined in Main.cpp or Utils.cpp)
// If you get a linker error, change "extern" to "AEGfxVertexList* pMeshSquare = 0;"

extern Platform BG_Layer1;
extern Platform BG_Layer2;

extern Player hamster;


AEGfxVertexList* pMeshSquare = nullptr;

void Level1_Load()
{
    // The player is invisible because the "Square" shape didn't exist in memory.
    // We must define the 2 triangles that make up the square.

    AEGfxMeshStart();

    // Triangle 1
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Triangle 2
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Save the shape to the pointer
    pMeshSquare = AEGfxMeshEnd();

    // --- END CHANGE 1 ---
    pTexPlatform = AEGfxTextureLoad("Assets/ground_green.png");
    if (!pTexPlatform) printf("ERROR: Failed to load Platform texture!\n");
    Player_Load(&hamster);

    Player_Load(&hamster);
    Environment_Load();
    Hazard_Load();
}

void Level1_Init()
{
    Player_Init(&hamster, -200.0f, 0.0f);
    Environment_Init();
    Hazard_Init();

    // Floor
    Platforms[0].Current_State = 1;
    Platforms[0].position.x = 0.0f;
    Platforms[0].position.y = -450.0f;
    Platforms[0].scale.x = 1800.0f;
    Platforms[0].scale.y = 80.0f;

    float platformPositions[5][2] = {
        { -400.0f,  -300.0f },
        {  400.0f,  -300.0f },
        {    0.0f,    -150.0f },
        { -600.0f, -150.0f },
        {  600.0f, -150.0f }
    };

    for (int i = 0; i < 5; ++i) {
        int a = i + 1;
        Platforms[a].Current_State = 1;
        Platforms[a].position.x = platformPositions[i][0]; // First column is X
        Platforms[a].position.y = platformPositions[i][1]; // Second column is Y
        Platforms[a].scale.x = 300.0f;
        Platforms[a].scale.y = 50.0f;
        Platforms[a].rotation = 0.0f;
        Platforms[a].pTex = pTexPlatform;
        //Platforms[0].pTex = pTexGround; // Use the ground texture loaded in Environment_Load
    }

}

void Level1_Update()
{
    // 1. Update Player Physics (Gravity/Input)
    Player_Update(&hamster);

    // 2. Update Environment (Moving platforms, etc.)
    Environment_Update();

    //3. Update Hazards (Meteor)
    Hazard_Update(g_dt);

    // 2. CHECK COLLISIONS (Player vs Meteors)
    std::vector<Meteor>& meteors = GetMeteors();
    for (int i = 0; i < (int)meteors.size(); ++i) {
        if (!meteors[i].active) continue;

        // Simple Distance Check
        // If distance between Player and Meteor is small -> GAME OVER
        float dx = hamster.position.x - meteors[i].x;
        float dy = hamster.position.y - meteors[i].y;
        float dist = sqrt(dx * dx + dy * dy);

        // 40.0f is a rough hit radius
        if (dist < 40.0f) {
            printf("HIT BY METEOR!\n");
            nextState = GS_RESTART; // Restart Level
        }
    }
    // COLLISION LOGIC

    // Reset Grounded State (Assume falling until we hit a floor)
    hamster.isGrounded = 0;

    // Define Player AABB
    AABB playerAABB;
    playerAABB.min.x = hamster.position.x - (hamster.scale.x / 2.0f);
    playerAABB.max.x = hamster.position.x + (hamster.scale.x / 2.0f);
    playerAABB.min.y = hamster.position.y - (hamster.scale.y / 2.0f);
    playerAABB.max.y = hamster.position.y + (hamster.scale.y / 2.0f);

    // Check collision against all platforms
    for (int i = 0; i < GET_PLATFORM_INSTANCE_NUM; ++i)
    {
        // Skip inactive platforms
        if (Platforms[i].Current_State == 0) continue;

        // Define Platform AABB
        AABB platformAABB;
        platformAABB.min.x = Platforms[i].position.x - (Platforms[i].scale.x / 2.0f);
        platformAABB.max.x = Platforms[i].position.x + (Platforms[i].scale.x / 2.0f);
        platformAABB.min.y = Platforms[i].position.y - (Platforms[i].scale.y / 2.0f);
        platformAABB.max.y = Platforms[i].position.y + (Platforms[i].scale.y / 2.0f);

        // Check Intersection
        if (CollisionIntersection_RectRect(playerAABB, platformAABB))
        {
            // Landing Logic:
            // Only land if falling (velocity <= 0) AND player is physically above the platform.
            if (hamster.velocity.y <= 0 && hamster.position.y > Platforms[i].position.y)
            {
                // Calculate the top surface of the platform
                float platformTop = Platforms[i].position.y + (Platforms[i].scale.y / 2.0f);
                float playerHalfHeight = hamster.scale.y / 2.0f;

                // Snap Player to sit exactly on top
                hamster.position.y = platformTop + playerHalfHeight;

                // Stop falling
                hamster.velocity.y = 0;

                // Allow Jumping again
                hamster.isGrounded = 1;
            }
        }
    }

    // Lose Condition: Falling off screen
    if (hamster.position.y < -400.0f)
    {
        nextState = GS_RESTART;
    }

    // Quit manually
    //if (AEInputCheckTriggered(AEVK_ESCAPE))
    //{
    //    nextState = GS_MENU;
    //}

    goToSettings(currentState);
}

void Level1_Draw()
{
    Environment_Draw();
    //Set a background color so we can see the player (White on White = Invisible)
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Black

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND); //enable blending

    AEGfxTextureSet(pTexPlatform, 0, 0);

    for (int i = 0; i < GET_PLATFORM_INSTANCE_NUM; ++i)
    {
        if (Platforms[i].Current_State == 0) continue;

        AEGfxTextureSet(Platforms[i].pTex, 0, 0);

        AEMtx33 scale, rot, trans, result;
        AEMtx33Scale(&scale, Platforms[i].scale.x, Platforms[i].scale.y);
        AEMtx33Rot(&rot, Platforms[i].rotation);
        AEMtx33Trans(&trans, Platforms[i].position.x, Platforms[i].position.y);

        AEMtx33Concat(&result, &rot, &scale);
        AEMtx33Concat(&result, &trans, &result);

        AEGfxSetTransform(result.m);
        AEGfxMeshDraw(pMeshSquare, AE_GFX_MDM_TRIANGLES);
    }

    Hazard_Draw();
    Player_Draw(&hamster);
}

void Level1_Free()
{
    // Free level specific resources
}

void Level1_Unload()
{
    // Free the mesh from the GPU
    if (pMeshSquare) {
        AEGfxMeshFree(pMeshSquare);
        pMeshSquare = 0; // Reset pointer to be safe
    }

    if (pTexPlatform) { AEGfxTextureUnload(pTexPlatform); pTexPlatform = 0; }

    Environment_Unload();
    Hazard_Unload();
}

bool IsGameOver() {
    // Return true if player position is below screen
    return (hamster.position.y < -450.0f);
}

bool IsLevelWon() {
    // Placeholder: Return false for now (or implement timer logic)
    return false;
}