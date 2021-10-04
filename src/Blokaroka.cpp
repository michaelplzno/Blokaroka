#include "Blokaroka.h"

// -- EXTERN DECLARATIONS ----------------------------------------------- //
volatile bool g_bIsAppAlive = true;
Config CONFIG;

double g_dFrequency;   // ticks per second
LARGE_INTEGER g_liLastFrame;   // counter tick of the last frame
float g_fDeltaT;               // elapsed time between frames


// -- MAIN -------------------------------------------------------------- //
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR pstrCmdLine, int iCmdShow)
{
    const char UniqueMutex[] = "one_Instance_only";
    HANDLE hHandle = CreateMutex(NULL, TRUE, UniqueMutex);

    if(ERROR_ALREADY_EXISTS == GetLastError())
    {
        return 1;
    }

    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);

    g_dFrequency = double(li.QuadPart);
    


    // - - - Command Line- - - - - - - //
    if (gHandleCommandLineArgs(pstrCmdLine) == false)
    {
        return 0;
    }

    RENDER.InitRenderer(hInstance);
    PHYSICS.Init();


    // Try to read the gamestate from the best existing gamestate location.
    if (!GAMESTATE.ReadGamestate(gGetExistingSaveFilePath()))
    {
        // Gamestate was not found or it could not be read correctly, generate a new randomized gamestate.
        GAMESTATE.GenerateBloks();
    }


    RENDER.RenderFrame();
    RENDER.PresentFrame();


    QueryPerformanceCounter(&g_liLastFrame);
    while(g_bIsAppAlive)
    {
        LARGE_INTEGER nextTime;
        QueryPerformanceCounter(&nextTime);

        g_fDeltaT = (float)((nextTime.QuadPart - g_liLastFrame.QuadPart) / g_dFrequency);
        g_liLastFrame = nextTime;

        RENDER.HandleWindows();
        GAMESTATE.Update();
        PHYSICS.UpdateMouse();
        PHYSICS.Update();
        RENDER.RenderFrame();
        RENDER.PresentFrame();
        Sleep(1);
    }

    RENDER.Shutdown();
    
    ReleaseMutex(hHandle);
    CloseHandle(hHandle);

    // Save the gamestate in the location where it exists, or the best place for it.
    GAMESTATE.DumpGamestate(gGetExistingSaveFilePath());

    return 0;
}

// -- handleCommandLineArgs --------------------------------------------- //
/*      Command Line Argument Handling (Loop and look for args that matter)
 */
bool gHandleCommandLineArgs(PSTR pstrCmdLine)
{
    return true;
}
