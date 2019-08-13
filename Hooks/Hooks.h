#ifndef __Hooks_H__
#define __Hooks_H__

#include <Windows.h>
#include <iostream>

__declspec(dllexport) int SetHook(HWND hookWnd);

// The 3 hooks needed to get desktop redraws

__declspec(dllexport) LRESULT GetMessageHook(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) LRESULT CallWndProcHook(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) LRESULT CallWndProcRetHook(int nCode, WPARAM wParam, LPARAM lParam);





#endif