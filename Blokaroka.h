////////////////////////////////////////////////////////////////////////////
//  Blokaroka.h                                                           //
//    A way to render interactive Bloks you can play with on your         //
//  Blokaroka.h                                                           //
//    A way to render interactive Bloks you can play with on your         //
//    windows desktop.                                                    //
//                                                                        //
//  Michael Silverman, 2007 - Open Source in 2019                         //
////////////////////////////////////////////////////////////////////////////

#ifndef __BLOKAROKA_H__
#define __BLOKAROKA_H__

#include "resource.h"

#include <time.h>
#include <vector>
#include <list>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sstream>

#include "box2d\box2d.h"


#define WINVER 0x0600
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600

#define WM_TRAY_MESSAGE (WM_USER + 1)

#include <windows.h>
#include <windowsx.h>
#include <Shlobj.h>

#pragma comment(lib, "Winmm.lib") 

#define MENU_BASIC      1000
#define MENU_AMETHYST   1001
#define MENU_TOPAZ      1002
#define MENU_PERIDOT    1003

#define MENU_RESET      1004
#define MENU_SAVE       1005
#define MENU_LOAD       1006

#define MENU_EXIT       1007

inline void gPlaySound(int resource)
{
    PlaySound(MAKEINTRESOURCE(resource), NULL, SND_RESOURCE|SND_ASYNC);
}

inline std::wstring gAppDataDirectory()
{
    // Search for the AppData folder for this user.
    wchar_t* knownPath = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_AppDataProgramData, KF_FLAG_CREATE, NULL, &knownPath);
    std::wstringstream wss;

    if (SUCCEEDED(hr))
    {
        OutputDebugString("Got Folder Path: ");

        wss << knownPath << L"\\Blokaroka\\";

        OutputDebugStringW(wss.str().c_str());
        OutputDebugString("\n");
    }
    else
    {
        wss << L".\\";
        OutputDebugString("NO Folder Path\n");
    }

    // Free the known path info.
    CoTaskMemFree(knownPath);

    return wss.str();
}

// Returns %APPDATA%\Blokaroka\\gamestate.blok as a std::string - This is the default desired save file location.
inline std::wstring gGetAppDataSaveFilePath()
{
    std::wstringstream wss;
    wss << gAppDataDirectory() << L"gamestate.blok";

    return wss.str();
}

// This function searches several locations for an existing save file and if it finds the file returns the path as a std::string.
// These are the locations it searches in order:
//   1) %APPDATA%\Blokaroka\\gamestate.blok
//   2) gamestate.blok
//   3) gamestate.db
// If the no files can be found it will return the first path.
inline std::wstring gGetExistingSaveFilePath()
{
    // ifstream to test if a file can be opened.
    std::ifstream in;

    // Test the AppData path.
    std::wstring path1 = gGetAppDataSaveFilePath();

    in.open(path1.c_str(), std::ofstream::in);
    if (in.is_open())
    {
        in.close();
        return path1;
    }

    // Test the .blok local path.
    std::wstring path2 = L"gamestate.blok";

    in.open(path2.c_str(), std::ofstream::in);
    if (in.is_open())
    {
        in.close();
        return path2;
    }

    // Test the .db legacy local path.
    std::wstring path3 = L"gamestate.db";

    in.open(path3.c_str(), std::ofstream::in);
    if (in.is_open())
    {
        in.close();
        return path3;
    }

    // return the default case since no file was found.
    return path1;
}


// -- COMMAND LINE ARGUMENTS -------------------------------------------- //
class Config {
  public:
    unsigned int m_uiWindowWidth;  // width of window
    unsigned int m_uiWindowHeight; // height of window
    unsigned int m_uiRandomSeed;   // random number generator seed
    bool m_bWindowedMode;          // windowed or not
    bool m_bRenderBackgroundGradient;
    bool m_bUseAlpha;              // For pseudo transparent objects
    bool m_bUseAA;                 // Anti-Aliasing
    bool m_bVSyncLock;             // Lock FPS to VSync


    Config() :
        m_uiWindowWidth(1024),
        m_uiWindowHeight(768),
        m_uiRandomSeed(1),
        m_bWindowedMode(true),
        m_bRenderBackgroundGradient(true),
        m_bUseAlpha(true),
        m_bUseAA(false),
        m_bVSyncLock(false)
    { };
};

extern volatile bool      g_bIsAppAlive;
extern Config             CONFIG;

extern LARGE_INTEGER g_liFrequency;   // ticks per second
extern LARGE_INTEGER g_liLastFrame;   // counter tick of the last frame
extern float g_fDeltaT;               // elapsed time between frames

// -- GLOBAL FUNCTIONS -------------------------------------------------- //
bool gHandleCommandLineArgs(PSTR pstrCmdLine);


// -- Project Classes --------------------------------------------------- //
class GameState;
class Blok;

#include "Blok.h"
#include "Renderer.h"
#include "Gamestate.h"
#include "Physics.h"

#endif
#endif
