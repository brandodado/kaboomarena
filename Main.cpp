#include "Main.hpp"
#include "AEEngine.h"
#include "Levels.hpp"
#include "Settings.hpp"
#include "Mainmenu.hpp"
#include "Player.hpp"
#include <stdio.h>

// Globals
float g_dt = 0.0f;
GameState currentState = GS_MENU;
GameState nextState = GS_MENU;
GameState prevState;
int gGameRunning = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    if (AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL) == 0)
        return 0;

    Level1_Load();
    mainmenu_init();
    Settings_Load();

    while (gGameRunning)
    {
        // 1. STATE MANAGEMENT
        // If the state changed (either by 'nextState' OR 'currentState'), handle it.
        if (nextState != currentState || nextState == GS_RESTART)
        {
            currentState = nextState;

            if (currentState == GS_PLAYING) Level1_Init();
            else if (currentState == GS_RESTART) {
                Level1_Init();
                currentState = GS_PLAYING;
                nextState = GS_PLAYING;
            }
            else if (currentState == GS_QUIT) gGameRunning = 0;
        }

        AEFrameRateControllerReset();
        g_dt = (float)AEFrameRateControllerGetFrameTime();

        AESysFrameStart();
        //AEInputUpdate();

        switch (currentState)
        {
        case GS_MENU:

            AEGfxSetCamPosition(0, 0);

            mainmenu_update();

            if (currentState != GS_MENU) {
                nextState = currentState;
            }

            mainmenu_draw();
            break;

        case GS_PLAYING:
            Level1_Update();

            AEGfxSetCamPosition(hamster.position.x, hamster.position.y);

            Level1_Draw();
            //if (AEInputCheckTriggered(AEVK_ESCAPE)) nextState = GS_MENU;
            break;

        case GS_GAMEOVER:
            // gameover_draw();
            if (AEInputCheckTriggered(AEVK_R)) nextState = GS_RESTART;
            break;

        case GS_HOWTOPLAY:
            // howtoplay_update();
            // howtoplay_draw();
            if (AEInputCheckTriggered(AEVK_ESCAPE)) nextState = GS_MENU;
            break;

        case GS_SETTINGS:
            Settings_Update();
            Settings_Draw();
            break;
        }

        AESysFrameEnd();
    }

    Level1_Unload();
    Settings_Unload();
    AESysExit();
    return 1;
}