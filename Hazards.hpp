#pragma once
#include "AEEngine.h"
#include <vector>

struct Meteor {
    float x, y;
    float velocityY;
    float size;
    bool active;
};

// Lifecycle
void Hazard_Load();
void Hazard_Init();
void Hazard_Update(float dt);
void Hazard_Draw();
void Hazard_Unload();

// Get the list of meteors so Player can check collision
std::vector<Meteor>& GetMeteors();