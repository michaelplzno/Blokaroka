////////////////////////////////////////////////////////////////////////////
//  DesktopBricks.h                                                       //
//    A way to render interactive bricks you can play with on your        //
//    windows desktop.                                                    //
//                                                                        //
//  Michael Silverman, 2007 - Open Source in 2019                         //
////////////////////////////////////////////////////////////////////////////

#ifndef __DESKTOPBRICKS_H__
#define __DESKTOPBRICKS_H__

#include <time.h>
#include <vector>
#include <list>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <windows.h>

#pragma comment(lib, "Winmm.lib") 
inline void gPlaySound(char * strFilename)
{
    char buff[400];
    sprintf_s(buff,400,"Sounds\\%s.wav", strFilename);
    PlaySound(buff, NULL, SND_FILENAME|SND_ASYNC);
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

// -- GLOBAL FUNCTIONS -------------------------------------------------- //
bool gHandleCommandLineArgs(PSTR pstrCmdLine);


// -- Fractal Shader Classes -------------------------------------------- //
#include "Renderer.h"
#include "Gamestate.h"

#endif