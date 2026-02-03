#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "AEInput.h"
#include "AEGraphics.h"
#include "Utils.hpp"
#include "Main.hpp"

extern GameState currentState;
extern GameState nextState;
extern GameState prevState;
extern int gGameRunning;
s8 gFont = -1;

//mesh, pointers to shapes drawn
AEGfxVertexList* normalButton = 0;
AEGfxVertexList* hoverButton = 0;

//size and positionings for buttons
AEVec2 startButtonPos, howToPlayButtonPos, settingsButtonPos;
float buttonWidth = 270.0f;
float buttonHeight = 75.0f;

//button colours
static unsigned int normalCol = 0xFFBD4AA8;
static unsigned int hoverCol = 0xFFF272DF;

//print text
void printText(s8 font, const char* text, AEVec2 pos) {
	AEGfxPrint(font, text, pos.x, pos.y, 1.f, 1.f, 1.f, 1.f, 1.f);
}

bool isHover(AEVec2 p, AEVec2 c) {
	return PointInRect(p, c, buttonWidth, buttonHeight);
}

void drawButton(AEVec2 pos, bool hover) {
	if (hover) {
		DrawMesh(hoverButton, pos, buttonWidth, buttonHeight);
	}
	else {
		DrawMesh(normalButton, pos, buttonWidth, buttonHeight);
	}
}

void mainmenu_init() {
	normalButton = sqrMesh(normalCol);
	hoverButton = sqrMesh(hoverCol);

	gFont = AEGfxCreateFont("Assets/buggy-font.ttf", 23);

	if (!normalButton || !hoverButton || gFont < 0) {
		printf("Failed to create mesh");
		gGameRunning = 0;
	}

	startButtonPos = { 0, 100.f };
	howToPlayButtonPos = { 0, 0.f };
	settingsButtonPos = { 0, -100.f };
}

void mainmenu_update() {

	//get position of players cursor
	s32 cX, cY;
	AEInputGetCursorPosition(&cX, &cY);
	AEVec2 cursor;
	cursor.x = (float)(cX - 800.f);
	cursor.y = (float)(450.f - cY);

	bool startHover = isHover(cursor, startButtonPos);
	bool howToPlayHover = isHover(cursor, howToPlayButtonPos);
	bool settingsHover = isHover(cursor, settingsButtonPos);

	outOfSettings();

	bool leftClick = AEInputCheckCurr(AEVK_LBUTTON);
	//bool leftClick = AEInputCheckCurr(AEVK_LBUTTON); //using this for now cause AEInputCheckTriggered isn't working :((
	if (leftClick) {
		if (startHover) nextState = GS_PLAYING;
		else if (howToPlayHover) nextState = GS_HOWTOPLAY;
		else if (settingsHover) nextState = GS_SETTINGS;
	}
}

void mainmenu_draw() {
	AEGfxSetBackgroundColor(0.18f, 0.97f, 1.f);

	//get position of players cursor
	s32 cX, cY;
	AEInputGetCursorPosition(&cX, &cY);
	AEVec2 cursor;
	cursor.x = (float)(cX - 800.f);
	cursor.y = (float)(450.f - cY);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//DrawMesh(normalButton, cursor, 10.f, 10.f); --> to see where my cursor is
	drawButton(startButtonPos, isHover(cursor, startButtonPos));
	drawButton(howToPlayButtonPos, isHover(cursor, howToPlayButtonPos));
	drawButton(settingsButtonPos, isHover(cursor, settingsButtonPos));

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	printText(gFont, "Play", {-0.05f, startButtonPos.y/400 - 0.05f});
	printText(gFont, "How To Play", {-0.155f, howToPlayButtonPos.y/400 - 0.025f});
	printText(gFont, "Settings", {-0.114f, settingsButtonPos.y/400});
}

void mainmenu_free() {
	if (normalButton) AEGfxMeshFree(normalButton);
	if (hoverButton) AEGfxMeshFree(hoverButton);
	if (gFont >= 0) AEGfxDestroyFont(gFont);
}

void gameover_draw() {

}