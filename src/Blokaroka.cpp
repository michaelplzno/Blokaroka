#include "Blokaroka.h"
#include "GameState.h"
#include "Physics.h"
#include "Renderer.h"

// -- EXTERN DECLARATIONS ----------------------------------------------- //
volatile bool g_bIsAppAlive = true;
Config CONFIG;

double g_dFrequency;         // ticks per second
LARGE_INTEGER g_liLastFrame; // counter tick of the last frame
float g_fDeltaT;             // elapsed time between frames
float g_fElapsedTime = 0.0f; // total elapsed time since start

// -- MAIN -------------------------------------------------------------- //
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR pstrCmdLine, int iCmdShow)
{
    const wchar_t UniqueMutex[] = L"one_instance_only_some_num_1982756";
    HANDLE hHandle = CreateMutexW(NULL, TRUE, UniqueMutex);

    if (ERROR_ALREADY_EXISTS == GetLastError())
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
        // Gamestate was not found or it could not be read correctly, generate a
        // new randomized gamestate.
        GAMESTATE.GenerateRandomBloks();
    }

    QueryPerformanceCounter(&g_liLastFrame);

    bloksPopping();

    // Main Game Loop, continue to loop while alive and Handle Windows, Update,
    // Physics, Render, and Present, then sleep a bit to not hog the processor.
    while (g_bIsAppAlive)
    {
        LARGE_INTEGER nextTime;
        QueryPerformanceCounter(&nextTime);

        g_fDeltaT = (float)((nextTime.QuadPart - g_liLastFrame.QuadPart) /
                            g_dFrequency);
        g_liLastFrame = nextTime;

        g_fElapsedTime += g_fDeltaT; // update elapsed time

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

    // Save the gamestate in the location where it exists, or the best place for
    // it.
    GAMESTATE.DumpGamestate(gGetExistingSaveFilePath());

    return 0;
}

// -- handleCommandLineArgs --------------------------------------------- //
/* Command Line Argument Handling (Loop and look for args that matter)    */
bool gHandleCommandLineArgs(PSTR pstrCmdLine)
{
    return true;
}

void bloksPopping()
{
    // setting every bloks size to minimum
    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); ++i)
    {
        GAMESTATE.m_vpoBloks[i]->SetWidth(1);
        GAMESTATE.m_vpoBloks[i]->SetHeight(4);
    }

    RENDER.RenderFrame();
    RENDER.PresentFrame();

    for (int size = 1; size < 15; ++size)
    {
        // adding 1 to width and height of ever blok
        for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); ++i)
        {
            GAMESTATE.m_vpoBloks[i]->SetWidth(
                GAMESTATE.m_vpoBloks[i]->GetWidth() + 1);

            GAMESTATE.m_vpoBloks[i]->SetHeight(
                GAMESTATE.m_vpoBloks[i]->GetHeight() + 1);
        }

        RENDER.RenderFrame();
        RENDER.PresentFrame();
        // sleep, so we can see the animation
        Sleep(15);
    }
}