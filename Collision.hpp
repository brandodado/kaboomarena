#pragma once
#include "AEEngine.h"

// --- CHANGE 1: DEFINE THE AABB STRUCT ---
// This is required by Levels.cpp and Player.cpp
typedef struct AABB
{
    AEVec2 min; // Bottom-Left corner (usually)
    AEVec2 max; // Top-Right corner (usually)
} AABB;

// --- CHANGE 2: DECLARE THE INTERSECTION FUNCTION ---
// This matches the function call in your Update loops
// Returns 1 if colliding, 0 if not.
int CollisionIntersection_RectRect(const AABB& aabb1, const AABB& aabb2);

// (Optional) Keep your old helper if you still use it elsewhere, 
// otherwise you can remove it.
bool CheckAABB(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2);