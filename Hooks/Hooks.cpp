//////////////////////////////////////////////////////////////////////////
//  Hooks.cpp                                                           //
//   A DLL of hooks to be called on desktop repaint. Based heavily on   //
//   code from:                                                         //
//                                                                      //
//      http://www.angelcode.com/dev/desktopdraw/                       //
//                                                                      //
//  This version by: Michael Silverman, 2008                            //
//////////////////////////////////////////////////////////////////////////

#include "Hooks.h"



// Global variables
HINSTANCE hDllModule = 0;

// We will share this data between all instances of the DLL
//#pragma data_seg(".HOOKDATA")
HWND hookWnd              = 0;
HWND ownerWnd             = 0;
HHOOK hHookCallWndProc    = 0;
HHOOK hHookGetMessage     = 0;
HHOOK hHookCallWndProcRet = 0;
// #pragma data_seg()
// #pragma comment(linker, "/SECTION:.HOOKDATA,RWS")


// -- DLLMAIN ----------------------------------------------------------- //
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Store the DLL's instance handle
        hDllModule = (HINSTANCE)hModule;
        MessageBox(NULL, "DLL MAIN", "OK", MB_OK);

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    if( !hDllModule )
        MessageBox(NULL, "DLL MAIN", "ERROR", MB_OK);
    return TRUE;
}

// Source: AngelCode.com
__declspec(dllexport) int SetHook(HWND inhookWnd, HWND inownerWnd)
{
    MessageBox(NULL, "Set Hook", "OK", MB_OK);
    if( hookWnd || ownerWnd )
        return -1;

    hookWnd = inhookWnd;
    ownerWnd = inownerWnd;

    hHookGetMessage = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMessageHook, hDllModule, 0); 
    hHookCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProcHook, hDllModule, 0);
    hHookCallWndProcRet = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)CallWndProcRetHook, hDllModule, 0); 

    if( !hHookCallWndProcRet || !hHookGetMessage || !hHookCallWndProc) 
        return -1;

    return 0;
}

__declspec(dllexport) LRESULT GetMessageHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

__declspec(dllexport) LRESULT CallWndProcHook(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    CWPSTRUCT cwp = *(PCWPSTRUCT)lParam;

    std::cout << "yo" << std::endl;
    //


    if( cwp.message == WM_KEYDOWN)
        MessageBox( NULL, TEXT("ok"), TEXT("OK"), MB_OK );
    int i = 0;
    i += 10;

    // Call next hook
    return CallNextHookEx( NULL, nCode, wParam, lParam );
}

__declspec(dllexport) LRESULT CallWndProcRetHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    return 0;
}




