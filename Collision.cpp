#include "Collision.hpp"
#include <math.h>

// --- REQUIRED FOR PLAYER.CPP ---
// This implements the function defined in Collision.hpp
int CollisionIntersection_RectRect(const AABB& aabb1, const AABB& aabb2)
{
    // Lecture 7 Logic (Slide 47)
    // 1. Is A completely to the Left of B?
    if (aabb1.max.x < aabb2.min.x) return 0;

    // 2. Is A completely to the Right of B?
    if (aabb1.min.x > aabb2.max.x) return 0;

    // 3. Is A completely Below B?
    if (aabb1.max.y < aabb2.min.y) return 0;

    // 4. Is A completely Above B?
    if (aabb1.min.y > aabb2.max.y) return 0;

    // If no gaps, they collide
    return 1;
}

// --- OPTIONAL HELPER ---
// You can keep this if you use it in other parts of your code, 
// but Player.cpp now uses the one above.
bool CheckAABB(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    float halfW1 = w1 / 2.0f;
    float halfH1 = h1 / 2.0f;
    float halfW2 = w2 / 2.0f;
    float halfH2 = h2 / 2.0f;

    if (x1 + halfW1 < x2 - halfW2) return false;
    if (x1 - halfW1 > x2 + halfW2) return false;
    if (y1 + halfH1 < y2 - halfH2) return false;
    if (y1 - halfH1 > y2 + halfH2) return false;

    return true;
}