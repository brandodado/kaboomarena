#pragma once
#include "AEEngine.h"

struct Vec2 { float x, y; };

struct Player
{
    Vec2 position;
    Vec2 velocity;
    Vec2 scale;
    int  isGrounded;
    int  jumpsRemaining;
    AEGfxVertexList* pMesh;
};

extern Player hamster;

// Functions
void Player_Load(Player* pPlayer);
void Player_Init(Player* pPlayer, float startX, float startY);
void Player_Update(Player* pPlayer);
void Player_Draw(Player* pPlayer);
void Player_Free(Player* pPlayer);