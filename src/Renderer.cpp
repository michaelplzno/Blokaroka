#include "Renderer.h"

b2Vec2 Renderer::Pixel::ToPhysics()
{
    b2Vec2 ret;
    ret.x = m_iX * PIXELS_TO_PHYSICS;
    ret.y = (RENDER.GetHeight() - m_iY) * PIXELS_TO_PHYSICS;
    return ret;
}

Renderer::Pixel::Pixel(b2Vec2 physicsPoint)
{
    SetFromPhysics(physicsPoint);
}

void Renderer::Pixel::SetFromPhysics(b2Vec2 physicsPoint)
{
    m_iX = (int)(physicsPoint.x * PHYSICS_TO_PIXELS);
    m_iY = RENDER.GetHeight() - (int)(physicsPoint.y * PHYSICS_TO_PIXELS);
}

typedef LRESULT(__cdecl *hookFunc)(HWND, HWND);

LRESULT CALLBACK MouseHookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (wParam == WM_LBUTTONUP)
    {
        PHYSICS.OnMouseUp();

        if (GAMESTATE.GetState() == GameState::GS_Drag_Free)
        {
            GAMESTATE.SetState(GameState::GS_Static);
            return 1;
        }

        if (GAMESTATE.GetState() == GameState::GS_Drag_Mating)
        {
            GAMESTATE.SetState(GameState::GS_Static);
            return 1;
        }
        return 0;
    }
    else if (wParam == WM_RBUTTONUP)
    {
        PHYSICS.OnMouseUp();

        if (GAMESTATE.GetState() != GameState::GS_Static)
        {
            RENDER.m_bRight = false;
            GAMESTATE.SetState(GameState::GS_Static);
            return 1;
        }
        if (RENDER.m_bRight)
        {
            RENDER.m_bRight = false;
            return 1;
        }
        return 0;
    }
    else if (wParam == WM_LBUTTONDOWN)
    {

        MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;

        int xPos = hook.pt.x;
        int yPos = hook.pt.y;

        tagPOINT result;
        GetPhysicalCursorPos(&result);
        xPos = result.x;
        yPos = result.y;

        Renderer::Pixel p(xPos, yPos);

        PHYSICS.OnMouseMove(p.ToPhysics());

        if (GAMESTATE.GetState() != GameState::GS_Drag_Free)
        {
            Renderer::Pixel p(xPos, yPos);

            Blok *poClicked = PHYSICS.OnMouseDown(p.ToPhysics());
            if (poClicked)
            {
                GAMESTATE.SetDragging(poClicked, xPos, yPos);
                return 1;
            }

            /*
            Blok * poClicked = GAMESTATE.GetBlokAt(xPos,yPos);
            if( poClicked )
            {
                GAMESTATE.SetDragging(poClicked, xPos, yPos);

                return 1;
            }
            */
        }
        return 0;
    }
    else if (wParam == WM_RBUTTONDOWN)
    {
        MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;
        int xPos = hook.pt.x;
        int yPos = hook.pt.y;

        tagPOINT result;
        GetPhysicalCursorPos(&result);
        xPos = result.x;
        yPos = result.y;

        Renderer::Pixel p(xPos, yPos);

        PHYSICS.OnMouseMove(p.ToPhysics());

        if (GAMESTATE.GetState() != GameState::GS_Drag_Free)
        {
            Blok *poClicked = GAMESTATE.GetBlokAt(xPos, yPos);
            int ret = 0;
            if (poClicked && poClicked->CanDetachUp())
            {
                poClicked->DetachUp();

                ret = 1;
            }
            else if (poClicked && poClicked->CanDetachDown())
            {
                poClicked->DetachDown();

                ret = 1;
            }
            else if (poClicked)
            {
                ret = 1;
            }

            if (ret == 1)
            {
                GAMESTATE.RegenPhysics();

                poClicked = PHYSICS.OnMouseDown(p.ToPhysics());
                if (poClicked)
                {
                    GAMESTATE.SetSplitDragging(poClicked, xPos, yPos);
                    RENDER.m_bRight = true;
                }

                return 1;
            }
        }
        return 0;
    }
    else if (wParam == WM_MOUSEMOVE)
    {
        tagPOINT result;
        GetPhysicalCursorPos(&result);
        int xPos = result.x;
        int yPos = result.y;

        if (GAMESTATE.GetState() == GameState::GS_Drag_Free ||
            GAMESTATE.GetState() == GameState::GS_Drag_Mating ||
            GAMESTATE.GetState() == GameState::GS_Drag_Splitting)
        {
            MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;

            Renderer::Pixel p(xPos, yPos);

            PHYSICS.OnMouseMove(p.ToPhysics());

            GAMESTATE.MoveSelected(xPos, yPos, xPos - RENDER.m_iLastMouseX,
                                   yPos - RENDER.m_iLastMouseY);
        }

        RENDER.m_iLastMouseX = xPos;
        RENDER.m_iLastMouseY = yPos;

        return 0;
    }

    /* if( wParam == WM_LBUTTONDOWN )
     {
         MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;

         if( hook.pt.x > 200 && hook.pt.x < 500 &&
             hook.pt.y > 3 && hook.pt.y < 500 )
         {
            RENDER.m_iThing = rand()  % 255;
            return 1;
         }
         else
         {
            return 0;
         }
     }*/

    // Call next hook
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyHookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{ // WH_MOUSE hook procedure

    if (!(nCode < 0))
    { // Process hook

        KBDLLHOOKSTRUCT hook = *(PKBDLLHOOKSTRUCT)lParam;

        switch (wParam)
        {
        case WM_KEYDOWN: {
            if (hook.vkCode == VK_ESCAPE)
            {
                PostQuitMessage(0);
                g_bIsAppAlive = false;

                return 1;
            }
        }
        }
    }

    // Call next hook
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

ITaskbarList3 *pTaskbarList = nullptr;

// Windows protocal handler function.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

    // Switch the windows message to figure out what it is
    switch (uMessage)
    {
    case WM_COMMAND: // User interacted with popup menu.
    {
        if (wParam == MENU_EXIT)
        {
            g_bIsAppAlive = false;
        }
        else if (wParam == MENU_RESET)
        {
            int answer =
                MessageBoxW(0,
                            L"Are you sure you want to reset your bloks? This "
                            L"cannot be undone.",
                            L"Reset Your Bloks?", MB_YESNO | MB_ICONQUESTION);
            if (answer == IDYES)
            {
                GAMESTATE.ClearBloks();
                GAMESTATE.GenerateRandomBloks();
                RENDER.RenderFrame();
                RENDER.PresentFrame();
            }
        }
        else if (wParam == MENU_SAVE)
        {
            OPENFILENAMEW ofn;       // common dialog box structure
            wchar_t szFile[260];     // buffer for file name
            HWND hwnd = RENDER.hWnd; // owner window

            // Initialize OPENFILENAME
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
            // use the contents of szFile to initialize itself.
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L".blok";
            ofn.lpstrDefExt = L"blok";
            ofn.nFilterIndex = 0;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST; // | OFN_FILEMUSTEXIST;

            // Display the Open dialog box.

            if (GetSaveFileNameW(&ofn) == TRUE)
            {

                int answer = IDYES;

                WIN32_FIND_DATAW FindFileData;
                HANDLE handle = FindFirstFileW(szFile, &FindFileData);

                if (handle != INVALID_HANDLE_VALUE)
                {
                    MessageBoxW(0,
                                L"This file already exists, are you sure you "
                                L"want to overwrite it?",
                                L"Overwrite Save File?",
                                MB_YESNO | MB_ICONQUESTION);
                }

                if (answer == IDYES)
                {
                    std::wstringstream wss;
                    wss << szFile;
                    GAMESTATE.DumpGamestate(wss.str());
                }
            }
        }
        else if (wParam == MENU_LOAD)
        {
            OPENFILENAMEW ofn;       // common dialog box structure
            wchar_t szFile[260];     // buffer for file name
            HWND hwnd = RENDER.hWnd; // owner window

            // Initialize OPENFILENAME
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
            // use the contents of szFile to initialize itself.
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L"Blok\0*.blok\0Text\0*.TXT\0All\0*.*";
            ofn.lpstrDefExt = L"blok";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // Display the Open dialog box.

            if (GetOpenFileNameW(&ofn) == TRUE)
            {
                std::wstringstream wss;
                wss << szFile;
                GAMESTATE.ClearBloks();
                GAMESTATE.ReadGamestate(wss.str());

                RENDER.RenderFrame();
                RENDER.PresentFrame();
            }
        }
        else if (wParam == MENU_BASIC || wParam == MENU_AMETHYST ||
                 wParam == MENU_TOPAZ || wParam == MENU_PERIDOT)
        {
            GAMESTATE.SetColorsForMenu(wParam);
        }
        else if (wParam == MENU_G_ON)
        {
            PHYSICS.ChangeGravity(-10.0f);
        }
        else if (wParam == MENU_G_OFF)
        {
            PHYSICS.ChangeGravity(0.0f);
        }
        else if (wParam == MENU_DEBUG_DRAW_ON)
        {
            PHYSICS.ToggleDebugRender();
        }
        else if (wParam == MENU_DEBUG_DRAW_OFF)
        {
            PHYSICS.ToggleDebugRender();
        }
        else if (LOWORD(wParam) == SUBMENU_GRAVITY)
        {
            PHYSICS.ToggleGravity();
        }
        else
        {
            // This case should never be reached.
            std::stringstream ss;
            ss << "You pressed option:" << wParam;
            std::string s = ss.str();
            std::wstring ws(s.begin(), s.end());
            MessageBoxW(NULL, ws.c_str(), L"Blockaroka Alert", MB_OK);
        }

        return 0;
    }

    case WM_TRAY_MESSAGE: // User interacted with the tray icon.
    {
        if (LOWORD(lParam) == WM_LBUTTONUP ||
            LOWORD(lParam) ==
                WM_RBUTTONUP) // User left or right clicked the tray icon.
        {
            // Generate a popup menu to ask the user what to do.
            RENDER.m_hPopupMenu = CreatePopupMenu();

            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_BASIC,
                        L"Basic Colors");
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_AMETHYST,
                        L"Amethyst");
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_TOPAZ, L"Topaz");
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_PERIDOT,
                        L"Peridot");
            AppendMenuW(RENDER.m_hPopupMenu, MF_SEPARATOR, 0, NULL);
            if (PHYSICS.IsGravityOn())
                AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_G_OFF,
                            L"Turn Gravity OFF");
            else
                AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_G_ON,
                            L"Turn Gravity ON");
            if (PHYSICS.IsDebugRenderOn())
                AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_DEBUG_DRAW_OFF,
                            L"Turn Debug Draw OFF");
            else
                AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_DEBUG_DRAW_ON,
                            L"Turn Debug Draw ON");
            AppendMenuW(RENDER.m_hPopupMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_RESET, L"Reset");
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_SAVE, L"Save");
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_LOAD, L"Load");
            AppendMenuW(RENDER.m_hPopupMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(RENDER.m_hPopupMenu, MF_STRING, MENU_EXIT, L"Exit");

            // Popup where the mouse is.
            tagPOINT pt;
            GetPhysicalCursorPos(&pt);

            // Display the popup.
            TrackPopupMenu(RENDER.m_hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           pt.x, pt.y, 0, hWnd, NULL);

            DestroyMenu(RENDER.m_hPopupMenu);
        }
        return 0;
    }

    case WM_KEYDOWN: // User pressed a key.
    {
        if (wParam == VK_ESCAPE) // Terminate program when ESC is pressed.
        {
            g_bIsAppAlive = false;
            return 1;
        }

        return 0;
    }

    case WM_PAINT: {
        return 0;
    }

    case WM_DESTROY: // The window is about to be closed
        // Clean up Taskbar interface
        if (pTaskbarList)
        {
            pTaskbarList->Release();
            pTaskbarList = nullptr;
        }
        PostQuitMessage(0);
    case WM_CLOSE: {
        // Our main window is closing.  This means we want our app to exit.
        g_bIsAppAlive = false;
        return 0;
    }

    default: // Some other message
    {
        // Let windows handle this message
        return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
    }
}

Renderer RENDER;

void Renderer::HandleWindows()
{
    // Deal with windows... we all have to
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            g_bIsAppAlive = false;
            return;
        }

        // Change the format of certain messages
        TranslateMessage(&msg);
        // Pass the message to WndProc() for processing
        DispatchMessage(&msg);
    }
}

void Renderer::RenderFrame(void)
{
    // HBRUSH NewBrush = CreateSolidBrush(CLEAR_COLOR);
    // HPEN NewPen = CreatePen(PS_SOLID, 3, CLEAR_COLOR);

    BYTE *pPixel = NULL;

    if (m_Image.width * 4 == m_Image.pitch)
    {
        /* This is a special case. When the image width is already a multiple
         * of 4 the image does not require any padding bytes at the end of each
         * scan line. Consequently we do not need to address each scan line
         * separately. This is much faster than the below case where the image
         * width is not a multiple of 4.
         */

        int i = 0;
        int totalBytes = m_Image.width * m_Image.height * 4;

        for (i = 0; i < totalBytes; i += 4)
        {
            pPixel = &m_Image.pPixels[i];
            pPixel[0] = 0;
            pPixel[1] = 0;
            pPixel[2] = 0;
            pPixel[3] = 0;
        }
    }
    else
    {
        /* Width of the image is not a multiple of 4. So padding bytes have
         * been included in the DIB's pixel data. Need to address each scan
         * line separately. This is much slower than the above case where the
         * width of the image is already a multiple of 4.
         */

        int x = 0;
        int y = 0;

        for (y = 0; y < m_Image.height; ++y)
        {
            for (x = 0; x < m_Image.width; ++x)
            {
                pPixel = &m_Image.pPixels[(y * m_Image.pitch) + (x * 4)];
                pPixel[0] = 0;
                pPixel[1] = 0;
                pPixel[2] = 0;
                pPixel[3] = 0;
            }
        }
    }

    // DeleteObject(NewPen);
    // DeleteObject(NewBrush);

    /*
    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        GAMESTATE.m_vpoBloks[i]->SetGroup(-1);
    }

    int index = 1;
    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        if (GAMESTATE.m_vpoBloks[i]->GetRenderState() == Blok::BRS_SOLID &&
    GAMESTATE.m_vpoBloks[i]->GetGroup() == -1)
        {
            GAMESTATE.m_vpoBloks[i]->RecursiveSetGroup(index);
            index++;
        }
    }

    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        if (GAMESTATE.m_vpoBloks[i]->GetGroup() == -1)
        {
            GAMESTATE.m_vpoBloks[i]->RecursiveSetGroup(index);
            index++;
        }
    }
    */

    GAMESTATE.Draw(m_HDC);

    PHYSICS.Draw();
}

void Renderer::PresentFrame()
{

    HGDIOBJ hPrevObj = NULL;
    POINT ptDest = {0, 0};
    POINT ptSrc = {0, 0};
    SIZE client = {m_Image.width, m_Image.height};
    BLENDFUNCTION blendFunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    hPrevObj = SelectObject(m_Image.hdc, m_Image.hBitmap);
    ClientToScreen(hWnd, &ptDest);

    if (UpdateLayeredWindow(hWnd, GetDC(hWnd), &ptDest, &client, m_Image.hdc,
                            &ptSrc, 0, &blendFunc, ULW_ALPHA) == 0)
    {
        char buff[300];
        sprintf_s(buff, 300, "Failed to update window: 0x%x", GetLastError());

        wchar_t wbuff[300];
        size_t convertedChars = 0;
        mbstowcs_s(&convertedChars, wbuff, buff, 300);
        MessageBoxW(NULL, wbuff, L"Error", MB_OK);
    }

    SelectObject(m_Image.hdc, hPrevObj);

    // clear zbufer;

    for (unsigned int i = 0; i < (unsigned int)(m_Width * m_Height); i++)
    {
        m_ausZBuffer[i] = 0;
    }

    // SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, m_Width, m_Height, SWP_SHOWWINDOW);

    /*
    POINT windowPosition;
    windowPosition.x = windowPosition.y = 0;
    POINT layerPosition = {0, 0};

    SIZE size;
    size.cx = m_Width;
    size.cy = m_Height;


    BLENDFUNCTION blendFunction= {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};


    if (UpdateLayeredWindow( hWnd, NULL,  &windowPosition,
        &size, m_HDC, &layerPosition, CLEAR_COLOR, &blendFunction, ULW_COLORKEY)
    == 0)
    {
        char buff[300];
        sprintf(buff, "Failed to update window: 0x%x", GetLastError());

        MessageBox(NULL, buff, "Error", MB_OK);
    }*/
}

void Renderer::InitRenderer(HINSTANCE hInstance)
{
    m_hInstance = hInstance;

    // ************************ WINDOWS INIT CODE ************************ \\
    //                                                                     \\

    m_bRight = false;

    WNDCLASSEXW wc; // The window class used to create our window

    // The name of our class and also the title to our window
    LPCWSTR strAppName = L"Blokaroka";        // App Name
    char strAppNameBuffer[128] = "Blokaroka"; // Tooltip text.

    // Fill in the window class with the attributes for our main window

    // The size of this struture in bytes
    wc.cbSize = sizeof(WNDCLASSEX);

    //  The style of the window.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    // Useless information.  Just set to zero.
    wc.cbClsExtra = 0;
    // Useless information.  Just set to zero.
    wc.cbWndExtra = 0;
    // The name of our event handler
    wc.lpfnWndProc = WndProc;
    // A handle to the applications instance
    wc.hInstance = hInstance;
    // The handle to the brush to use for the window background
    wc.hbrBackground = CreateSolidBrush(CLEAR_COLOR);
    // A handle to the icon to use for the window
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    // A handle to a smaller version of the apps icon
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    // A handle to the cursor to use while the mouse is over our window
    wc.hCursor = LoadCursor(NULL, IDC_HAND);
    // A handle to the resource to use as our menu
    wc.lpszMenuName = NULL;
    // The human readable name for this class
    wc.lpszClassName = strAppName;

    // Register the class with windows
    RegisterClassExW(&wc);

    // make sure the window is created with the correct game area as opposed to
    // correct outer size
    int xAdd, yAdd;
    xAdd = yAdd = 0;
    DWORD winType = WS_POPUP | WS_VISIBLE;
    DWORD exWinType = WS_EX_LAYERED | WS_EX_TOPMOST;

    // Create the window based on the previous class
    hWnd =
        CreateWindowExW(exWinType | WS_EX_APPWINDOW, // Advanced style settings
                        strAppName,                  // The name of the class
                        strAppName,                  // The window caption
                        winType,                     // The window style
                        0,                           // The initial x position
                        0,                           // The initial y position
                        1280,
                        1000,      // The initial width / height
                        NULL,      // Handle to parent window
                        NULL,      // Handle to the menu
                        hInstance, // Handle to the apps instance
                        NULL);     // Advanced context

    if (SetWindowsHookEx(13, KeyHookWndProc, GetModuleHandle(NULL), NULL) ==
        NULL)
    {
        MessageBox(NULL, TEXT("ok"), TEXT("Unable to load function(s)."),
                   MB_OK);
        g_bIsAppAlive = false;
        return;
    }

    if (SetWindowsHookEx(14, MouseHookWndProc, GetModuleHandle(NULL), NULL) ==
        NULL)
    {
        MessageBox(NULL, TEXT("ok"), TEXT("Unable to load function(s)."),
                   MB_OK);
        g_bIsAppAlive = false;
        return;
    }

    // Create the Taskbar interface
    HRESULT hr =
        CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER,
                         IID_PPV_ARGS(&pTaskbarList));
    if (FAILED(hr) || FAILED(pTaskbarList->HrInit()))
    {
        MessageBox(hWnd, TEXT("Failed to initialize Taskbar interface"),
                   TEXT("Error"), MB_ICONERROR);
        return;
    }

    // create Taskbar Thumbbutton image
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));

    // Define the Taskbar button
    THUMBBUTTON thumbButton = {};
    thumbButton.dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thumbButton.iId = SUBMENU_GRAVITY;
    thumbButton.hIcon = hIcon;
    for (int i = 0; i < 128; i++)
    {
        thumbButton.szTip[i] = strAppNameBuffer[i];
    }
    thumbButton.dwFlags = THBF_ENABLED;

    // Add the button to the taskbar thumbnail toolbar
    hr = pTaskbarList->ThumbBarAddButtons(hWnd, 1, &thumbButton);
    if (FAILED(hr))
    {
        MessageBox(hWnd, TEXT("Failed to add Taskbar button"), TEXT("Error"),
                   MB_ICONERROR);
    }

    /*
        HOOKPROC hkprcSysMsg;
        static HINSTANCE hinstDLL;
        static HHOOK hhookSysMsg;
        hookFunc SetHook = NULL;

        hinstDLL = LoadLibrary("Hooks.dll");
        if(!hinstDLL)
        {
            MessageBox(GetDesktopWindow(), "Could Not Load DLL", "Error",
       MB_OK);
        }

        SetHook = (hookFunc)GetProcAddress((HMODULE)hinstDLL, "SetHook");
        if(SetHook == NULL)
        {
            MessageBox(NULL, "Failed to load SetHook", "Error", MB_OK);
            g_bIsAppAlive = false;
            return;
        }

        int r = SetHook(GetDesktopWindow(), hWnd);
        if( r < 0 )
            MessageBox(GetDesktopWindow(), "Failed to install hook", "Error",
       MB_OK);

           hkprcSysMsg = (HOOKPROC)GetProcAddress((HMODULE)hinstDLL,
       (LPTSTR)"CallWndProc");


        hhookSysMsg = SetWindowsHookEx(WH_CALLWNDPROC,hkprcSysMsg,hinstDLL,0);

        SetHook(hhookSysMsg);

        if ( hkprcSysMsg == NULL || hhookSysMsg == NULL)
        {
            MessageBox( NULL, TEXT("ok"), TEXT("Unable to load function(s)."),
       MB_OK ); g_bIsAppAlive = false; return;
        }*/

    // Display the window we just created
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    // Draw the window contents for the first time

    m_HDC = GetDC(hWnd);

    RECT r;
    GetClipBox(GetDC(NULL), &r);

    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    // horizontal = desktop.right;
    // vertical = desktop.bottom;

    /*SetWindowLong(hWnd,
        GWL_EXSTYLE,
        GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);*/

    // Make this window 70% alpha
    // SetLayeredWindowAttributes(hWnd, CLEAR_COLOR, 255, LWA_COLORKEY);

    m_Width = desktop.right;   // r.right - r.left;
    m_Height = desktop.bottom; // r.bottom - r.top;

    m_ausZBuffer = new unsigned int[m_Width * m_Height];

    if (!ImageCreate(&m_Image, m_Width, m_Height))
    {
        MessageBoxW(NULL, L"Failed CreateCompatibleBitmap", L"Error", MB_OK);
        return;
    }

    RenderFrame();
    PresentFrame();

    m_tnd.cbSize = sizeof(NOTIFYICONDATA);
    m_tnd.hWnd = hWnd;
    m_tnd.uID = 1;
    m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tnd.uCallbackMessage = WM_TRAY_MESSAGE;
    m_tnd.dwState = NIS_SHAREDICON;
    m_tnd.uVersion = NOTIFYICON_VERSION_4;
    for (int i = 0; i < 128; i++)
    {
        m_tnd.szTip[i] = strAppNameBuffer[i];
    }
    m_tnd.hIcon = wc.hIconSm;

    Shell_NotifyIcon(NIM_ADD, &m_tnd);
}

void Renderer::SetPixel(int x, int y, COLORREF color, unsigned int depth,
                        float fAlpha /*= 1.0f*/, float fSourceBlend)
{
    if (x >= m_Image.width || x < 0 || y >= m_Image.height || y <= 0)
    {
        return;
    }

    if (m_ausZBuffer[y * m_Width + x] > depth)
    {
        return;
    }

    BYTE *pPixel = NULL;

    // if(fAlpha == 1.0f)
    {
        m_ausZBuffer[y * m_Width + x] = depth;
        if (m_Image.width * 4 == m_Image.pitch)
        {
            /* This is a special case. When the image width is already a
             * multiple of 4 the image does not require any padding bytes at the
             * end of each scan line. Consequently we do not need to address
             * each scan line separately. This is much faster than the below
             * case where the image width is not a multiple of 4.
             */

            int i = 0;
            int totalBytes = m_Image.width * m_Image.height * 4;

            pPixel = &m_Image.pPixels[x * 4 + y * m_Image.width * 4];
            pPixel[0] = (BYTE)(GetBValue(color) *
                               fAlpha); // * (BYTE)((float)pPixel[3] / 255.0f);
            pPixel[1] = (BYTE)(GetGValue(color) * fAlpha);
            pPixel[2] = (BYTE)(GetRValue(color) * fAlpha);
            pPixel[3] = (BYTE)(255 * fAlpha);
        }
        else
        {
            /* Width of the image is not a multiple of 4. So padding bytes have
             * been included in the DIB's pixel data. Need to address each scan
             * line separately. This is much slower than the above case where
             * the width of the image is already a multiple of 4.
             */

            pPixel = &m_Image.pPixels[(y * m_Image.pitch) + (x * 4)];
            pPixel[0] = (BYTE)(GetBValue(color) *
                               fAlpha); // * (BYTE)((float)pPixel[3] / 255.0f);
            pPixel[1] = (BYTE)(GetGValue(color) * fAlpha);
            pPixel[2] = (BYTE)(GetRValue(color) * fAlpha);
            pPixel[3] = (BYTE)(255 * fAlpha);
        }
    }
}

void Renderer::Rectangle(int left, int top, int right, int bottom,
                         COLORREF color, unsigned int depth,
                         float fAlpha /*= 1.0f*/)
{
    for (int x = left; x <= right; x++)
    {
        for (int y = top; y <= bottom; y++)
        {
            SetPixel(x, y, color, depth, fAlpha);
        }
    }
}

void Renderer::HorizontalLine(int left, int right, int y, COLORREF color,
                              unsigned int depth, float fAlpha /*= 1.0f*/)
{
    for (int x = left; x <= right; x++)
    {
        SetPixel(x, y, color, depth, fAlpha);
    }
}

void Renderer::VerticalLine(int top, int bottom, int x, COLORREF color,
                            unsigned int depth, float fAlpha /*= 1.0f*/)
{
    for (int y = top; y <= bottom; y++)
    {
        SetPixel(x, y, color, depth, fAlpha);
    }
}

void Renderer::Line(Renderer::Pixel start, Renderer::Pixel end, COLORREF color,
                    unsigned int depth, float alpha)
{
    Line(start.m_iX, start.m_iY, end.m_iX, end.m_iY, color, depth, alpha);
}

void Renderer::Line(int x0, int y0, int x1, int y1, COLORREF color,
                    unsigned int depth, float alpha)
{
    /* if (x1 < x0)
     {
         int temp = x1;
         x1 = x0;
         x0 = temp;

         temp = y1;
         y1 = y0;
         y0 = temp;
     }*/

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true)
    {
        SetPixel(x0, y0, color, depth, alpha);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy; /* e_xy+e_x > 0 */
            x0 += sx;
        }

        if (e2 <= dx) /* e_xy+e_y < 0 */
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Renderer::Circle(int xCenter, int yCenter, int radius, COLORREF c,
                      unsigned int depth /*= 0*/, float fAlpha /*= 1.0f*/)
{
    int x = 0;
    int y = radius;
    int p = (5 - radius * 4) / 4;

    circlePoints(xCenter, yCenter, x, y, c, depth, fAlpha);
    while (x < y)
    {
        x++;
        if (p < 0)
        {
            p += 2 * x + 1;
        }
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
        circlePoints(xCenter, yCenter, x, y, c, depth, fAlpha);
    }
}

void Renderer::circlePoints(int cx, int cy, int x, int y, COLORREF color,
                            unsigned int depth, float alpha)
{
    if (x == 0)
    {
        SetPixel(cx, cy + y, color, depth, alpha);
        SetPixel(cx, cy - y, color, depth, alpha);
        SetPixel(cx + y, cy, color, depth, alpha);
        SetPixel(cx - y, cy, color, depth, alpha);
    }
    else if (x == y)
    {
        SetPixel(cx + x, cy + y, color, depth, alpha);
        SetPixel(cx - x, cy + y, color, depth, alpha);
        SetPixel(cx + x, cy - y, color, depth, alpha);
        SetPixel(cx - x, cy - y, color, depth, alpha);
    }
    else if (x < y)
    {
        SetPixel(cx + x, cy + y, color, depth, alpha);
        SetPixel(cx - x, cy + y, color, depth, alpha);
        SetPixel(cx + x, cy - y, color, depth, alpha);
        SetPixel(cx - x, cy - y, color, depth, alpha);
        SetPixel(cx + y, cy + x, color, depth, alpha);
        SetPixel(cx - y, cy + x, color, depth, alpha);
        SetPixel(cx + y, cy - x, color, depth, alpha);
        SetPixel(cx - y, cy - x, color, depth, alpha);
    }
}

void Renderer::Shutdown()
{
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
}

void Renderer::ShowInfoMessage(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (strcmp(buffer, m_lastMessage) == 0 &&
        (g_fElapsedTime - m_fLastMessageTime) < 5.0f)
    {
        return;
    }

    strcpy_s(m_lastMessage, buffer);
    m_fLastMessageTime = g_fElapsedTime;

    MessageBoxA(NULL, buffer, "INFO", MB_OK | MB_ICONINFORMATION);
}