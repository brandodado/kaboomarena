#pragma once

// --- Game State Manager Lifecycle Functions ---
// These match the calls in Main.cpp and the definitions in Levels.cpp

void Level1_Load();
void Level1_Init();
void Level1_Update();
void Level1_Draw();
void Level1_Free();
void Level1_Unload();

// (Optional) Helper functions if you still use them inside Levels.cpp
// You can remove these if you moved the logic directly into Level1_Update
bool IsGameOver();
bool IsLevelWon();