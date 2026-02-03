#pragma once

// Game States
enum GameState {
	GS_MENU,
	GS_PLAYING,   // The Game / Level 1
	GS_HOWTOPLAY,
	GS_RESTART,   // Logic to reset the level
	GS_GAMEOVER,
	GS_SETTINGS,
	GS_QUIT
};

// Globals
extern GameState currentState;	
extern GameState nextState;
extern GameState prevState;
extern float g_dt; // Global Delta Time