#include "DesktopBricks.h"

// -- EXTERN DECLARATIONS ----------------------------------------------- //
volatile bool g_bIsAppAlive = true;
Config CONFIG;

// -- MAIN -------------------------------------------------------------- //
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR pstrCmdLine, int iCmdShow )
{ 

    // - - - Command Line- - - - - - - //
    if (gHandleCommandLineArgs(pstrCmdLine) == false)
    {
        return 0;
    }

    RENDER.InitRenderer(hInstance);
 

    if( !GAMESTATE.ReadGamestate("gamestate.db") )
    {
        GAMESTATE.GenerateBricks();
    }


    RENDER.RenderFrame();
    RENDER.PresentFrame();


    while(g_bIsAppAlive)
    {
        RENDER.HandleWindows();
        Sleep(1);
    }

    GAMESTATE.DumpGamestate("gamestate.db");

    return 0;
}

// -- handleCommandLineArgs --------------------------------------------- //
/*      Command Line Argument Handling (Loop and look for args that matter)
 */
bool gHandleCommandLineArgs(PSTR pstrCmdLine)
{
    return true;
}