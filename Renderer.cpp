#include "Renderer.h"
#include <math.h>


typedef LRESULT (__cdecl*hookFunc)(HWND, HWND);

LRESULT CALLBACK MouseHookWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    if( wParam == WM_LBUTTONUP )
    { 
        if(GAMESTATE.GetState() == GameState::GS_Drag_Free)
        {
            GAMESTATE.SetState(GameState::GS_Static);
            RENDER.RenderFrame();
            RENDER.PresentFrame();
            return 1;
        }

        if(GAMESTATE.GetState() == GameState::GS_Drag_Mating)
        {
            GAMESTATE.SetState(GameState::GS_Static);
            RENDER.RenderFrame();
            RENDER.PresentFrame();
            return 1;
        }
        return 0;
    }
    else if( wParam == WM_RBUTTONUP )
    { 
        if(GAMESTATE.GetState() != GameState::GS_Static)
        {
            RENDER.m_bRight = false;
            GAMESTATE.SetState(GameState::GS_Static);
            RENDER.RenderFrame();
            RENDER.PresentFrame();
            return 1;
        }
        if(RENDER.m_bRight)
        {
            RENDER.m_bRight = false;
            return 1;
        }
        return 0;
    }
    else if( wParam == WM_LBUTTONDOWN )
    {
        MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;
	
        int xPos = hook.pt.x; 
        int yPos = hook.pt.y; 


		tagPOINT result;
		GetPhysicalCursorPos(&result);
		xPos = result.x;
		yPos = result.y;

        if(GAMESTATE.GetState() != GameState::GS_Drag_Free)
        {
            GameState::Brick * poClicked = GAMESTATE.GetBrickAt(xPos,yPos);
            if( poClicked )
            {
                GAMESTATE.SetDragging(poClicked, xPos, yPos);
                
                return 1;
            }

        }
        return 0;
    }
    else if( wParam == WM_RBUTTONDOWN )
    {
        MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;
        int xPos = hook.pt.x; 
        int yPos = hook.pt.y; 

		tagPOINT result;
		GetPhysicalCursorPos(&result);
		xPos = result.x;
		yPos = result.y;

        if(GAMESTATE.GetState() != GameState::GS_Drag_Free)
        {
            GameState::Brick * poClicked = GAMESTATE.GetBrickAt(xPos,yPos);
            if( poClicked && poClicked->CanDetachBrickUp())
            {
                poClicked->DetachBrickUp();
                GAMESTATE.SetSplitDragging(poClicked, xPos, yPos);
                //GAMESTATE.SetState(GameState::GS_Drag_Free);
				RENDER.m_bRight = true;
                return 1;
            }

            if( poClicked && poClicked->CanDetachBrickDown())
            {
                poClicked->DetachBrickDown();
                GAMESTATE.SetSplitDragging(poClicked, xPos, yPos);
				RENDER.m_bRight = true;
                return 1;
            }

            if(poClicked)
            {
                GAMESTATE.SetSplitDragging(poClicked, xPos, yPos);
                //GAMESTATE.SetState(GameState::GS_Drag_Free);
                RENDER.m_bRight = true;
                return 1;
            }

        }
        return 0;
    }
    else if( wParam == WM_MOUSEMOVE )
    { 
        if(GAMESTATE.GetState() == GameState::GS_Drag_Free || 
           GAMESTATE.GetState() == GameState::GS_Drag_Mating ||
           GAMESTATE.GetState() == GameState::GS_Drag_Splitting )
        {
            MSLLHOOKSTRUCT hook = *(PMSLLHOOKSTRUCT)lParam;

			tagPOINT result;
			GetPhysicalCursorPos(&result);
			int xPos = result.x;
			int yPos = result.y;

            GAMESTATE.MoveSelected(xPos, yPos);

            RENDER.RenderFrame();
            RENDER.PresentFrame();
            return 0;
        }
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
    return CallNextHookEx( NULL, nCode, wParam, lParam );
}


LRESULT CALLBACK KeyHookWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{// WH_MOUSE hook procedure

    if (!(nCode < 0))
    {// Process hook

        KBDLLHOOKSTRUCT hook = *(PKBDLLHOOKSTRUCT)lParam;

        switch (wParam)
        {
        case WM_KEYDOWN: 
            { 
                if( hook.vkCode == VK_ESCAPE ) 
                {
                    PostQuitMessage( 0 );
                    g_bIsAppAlive = false;


                    return 1;
                }
            }
        }


    }

    // Call next hook
    return CallNextHookEx( NULL, nCode, wParam, lParam );
}

// Windows protocal handler function.
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMessage, 
                          WPARAM wParam, LPARAM lParam )
{

    // Switch the windows message to figure out what it is
    switch( uMessage )
    {
    case WM_KEYDOWN: 
        {
            if( wParam == VK_ESCAPE )
            {
                PostQuitMessage( 0 );
                g_bIsAppAlive = false;
                return 1;
            }
            else if( wParam == VK_RETURN )
            {
                //Terminal::enter( );
            }
            else if( wParam == VK_UP )
            {
                //Terminal::displayLastEntry( );
            }
            return 0;
        }

    case WM_MOUSEMOVE:
    {
            return 0;
    }

    case WM_LBUTTONDOWN:
    {
        
        return 0;
    }

    case WM_LBUTTONUP:
    {

        return 0;
    }

    case WM_SETCURSOR:
        {
        SetCursor(NULL);
        return 0;
        }


    case WM_PAINT:
        {
            return 0;
        }

    case WM_DESTROY:  	// The window is about to be closed
        {  
            // Our main window is closing.  This means we want our app to exit.
            // Tell windows to put a WM_QUIT message in our message queue
            //PostQuitMessage( 0 );
            return 0;
        }

    case WM_CLOSE:// The window is about to be closed
        {  
            // Our main window is closing.  This means we want our app to exit.
            // Tell windows to put a WM_QUIT message in our message queue
            //PostQuitMessage( 0 );
            return 0;
        }


    default:			// Some other message
        {
            // Let windows handle this message

            return DefWindowProc( hWnd, uMessage, wParam, lParam );
        }
    }
}

Renderer RENDER;


void Renderer::HandleWindows()
{
    // Deal with windows... we all have to
    MSG msg;
    if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {	
        if( msg.message == WM_QUIT )
        {
            g_bIsAppAlive = false;
            return;
        }

        // Change the format of certain messages
        TranslateMessage( &msg );
        // Pass the message to WndProc() for processing
        DispatchMessage( &msg );
    }
}

void Renderer::RenderFrame(void)
{
    HBRUSH NewBrush = CreateSolidBrush(CLEAR_COLOR);
    HPEN NewPen = CreatePen(PS_SOLID, 3, CLEAR_COLOR);

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
            pPixel[0] = 0;//(BYTE)((float)pPixel[3] / 255.0f);
            pPixel[1] = 0;//(BYTE)((float)pPixel[3] / 255.0f);
            pPixel[2] *= 0;
            pPixel[3]  = 0;//(BYTE)((float)pPixel[3] / 255.0f);
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
                pPixel[0] *= 0;//(BYTE)((float)pPixel[3] / 255.0f);
                pPixel[1] *= 0;//(BYTE)((float)pPixel[3] / 255.0f);
                pPixel[2] *= 0;
                pPixel[3]  = 0;//(BYTE)((float)pPixel[3] / 255.0f);
            }
        }
    }

    DeleteObject(NewPen);
    DeleteObject(NewBrush);

	for (unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++)
	{
		GAMESTATE.m_vpoBricks[i]->m_iGroup = -1;
	}

	int index = 1;
	for (unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++)
	{
		if (GAMESTATE.m_vpoBricks[i]->GetRenderState() == GameState::Brick::LRS_SOLID && GAMESTATE.m_vpoBricks[i]->m_iGroup == -1)
		{
			GAMESTATE.m_vpoBricks[i]->RecursiveSetGroup(index);
			index++;
		}
	}

	for (unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++)
	{
		if (GAMESTATE.m_vpoBricks[i]->m_iGroup == -1)
		{
			GAMESTATE.m_vpoBricks[i]->RecursiveSetGroup(index);
			index++;
		}
	}

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        GAMESTATE.m_vpoBricks[i]->DrawBrick(m_HDC);
    }
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

    if( UpdateLayeredWindow(hWnd, GetDC(hWnd), &ptDest, &client, 
                            m_Image.hdc, &ptSrc, 0, &blendFunc, ULW_ALPHA) == 0)
    {
        char buff[300];
        sprintf_s(buff, 300, "Failed to update window: 0x%x", GetLastError());

        MessageBox(NULL, buff, "Error", MB_OK);
    }

    SelectObject(m_Image.hdc, hPrevObj);

    // clear zbufer;

    for(unsigned int i = 0; i < (unsigned int)(m_Width * m_Height); i++)
    {
        m_ausZBuffer[i] = 0;
    }


    //SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, m_Width, m_Height, SWP_SHOWWINDOW);


    /*
    POINT windowPosition;
    windowPosition.x = windowPosition.y = 0;
    POINT layerPosition = {0, 0};

    SIZE size;
    size.cx = m_Width;
    size.cy = m_Height;


    BLENDFUNCTION blendFunction= {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};


    if (UpdateLayeredWindow( hWnd, NULL,  &windowPosition,
        &size, m_HDC, &layerPosition, CLEAR_COLOR, &blendFunction, ULW_COLORKEY) == 0)
    {
        char buff[300];
        sprintf(buff, "Failed to update window: 0x%x", GetLastError());

        MessageBox(NULL, buff, "Error", MB_OK);
    }*/
}



void Renderer::InitRenderer(HINSTANCE hInstance)
{
    // ************************ WINDOWS INIT CODE ************************ \\
    //                                                                     \\

    m_bRight = false;

    WNDCLASSEX wc;	// The window class used to create our window

    // The name of our class and also the title to our window
    char* strAppName = "Desktop Bricks" ;

    // Fill in the window class with the attributes for our main window

    // The size of this struture in bytes
    wc.cbSize			= sizeof( WNDCLASSEX ); 

    //  The style of the window.
    wc.style		    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    // Useless information.  Just set to zero.
    wc.cbClsExtra		= 0;
    // Useless information.  Just set to zero.
    wc.cbWndExtra		= 0;
    // The name of our event handler
    wc.lpfnWndProc		= WndProc;
    // A handle to the applications instance
    wc.hInstance		= hInstance;
    // The handle to the brush to use for the window background
    wc.hbrBackground	= CreateSolidBrush(CLEAR_COLOR);
    // A handle to the icon to use for the window
    wc.hIcon		    = LoadIcon( hInstance, MAKEINTRESOURCE(1) );
    // A handle to a smaller version of the apps icon
    wc.hIconSm			= LoadIcon( hInstance, MAKEINTRESOURCE(1) );
    // A handle to the cursor to use while the mouse is over our window
    wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
    // A handle to the resource to use as our menu
    wc.lpszMenuName	    = NULL;
    // The human readable name for this class
    wc.lpszClassName	= strAppName;

    // Register the class with windows
    RegisterClassEx( &wc );

    // make sure the window is created with the correct game area as opposed to correct outer
    // size
    int xAdd, yAdd;
    xAdd = yAdd = 0;
    DWORD winType = WS_POPUP|WS_VISIBLE;
    DWORD exWinType = WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST;


    // Create the window based on the previous class	
    hWnd = CreateWindowEx(  exWinType, 			    // Advanced style settings 
                            strAppName,             // The name of the class
                            strAppName,             // The window caption
                            winType,				// The window style
                            0,		                // The initial x position 
                            0,                      // The initial y position
                            1280,
                            1000,                   // The initial width / height
                            NULL,			        // Handle to parent window						
                            NULL,				    // Handle to the menu
                            hInstance,			    // Handle to the apps instance
                            NULL );				    // Advanced context



    if (SetWindowsHookEx( 13, KeyHookWndProc, GetModuleHandle(NULL), NULL ) == NULL)
    {
        MessageBox( NULL, TEXT("ok"), TEXT("Unable to load function(s)."), MB_OK );
        g_bIsAppAlive = false;
        return;
    }

    if (SetWindowsHookEx( 14, MouseHookWndProc, GetModuleHandle(NULL), NULL ) == NULL)
    {
        MessageBox( NULL, TEXT("ok"), TEXT("Unable to load function(s)."), MB_OK );
        g_bIsAppAlive = false;
        return;
    }

/* 
    HOOKPROC hkprcSysMsg; 
    static HINSTANCE hinstDLL; 
    static HHOOK hhookSysMsg; 
    hookFunc SetHook = NULL;

    hinstDLL = LoadLibrary("Hooks.dll"); 
    if(!hinstDLL)
    {
        MessageBox(GetDesktopWindow(), "Could Not Load DLL", "Error", MB_OK);
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
        MessageBox(GetDesktopWindow(), "Failed to install hook", "Error", MB_OK);

       hkprcSysMsg = (HOOKPROC)GetProcAddress((HMODULE)hinstDLL, (LPTSTR)"CallWndProc");
    

    hhookSysMsg = SetWindowsHookEx(WH_CALLWNDPROC,hkprcSysMsg,hinstDLL,0); 

    SetHook(hhookSysMsg);

    if ( hkprcSysMsg == NULL || hhookSysMsg == NULL)
    {
        MessageBox( NULL, TEXT("ok"), TEXT("Unable to load function(s)."), MB_OK );
        g_bIsAppAlive = false;
        return;
    }*/
    // Display the window we just created
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    // Draw the window contents for the first time

    m_HDC = GetDC(hWnd);



    RECT r;
    GetClipBox(GetDC(NULL),&r);

    /*SetWindowLong(hWnd, 
        GWL_EXSTYLE, 
        GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);*/

    // Make this window 70% alpha
    //SetLayeredWindowAttributes(hWnd, CLEAR_COLOR, 255, LWA_COLORKEY);


    m_Width = r.right - r.left;
    m_Height = r.bottom - r.top;

    m_ausZBuffer = new unsigned int[m_Width * m_Height];

    if (!ImageCreate(&m_Image, m_Width, m_Height))
    {
        MessageBox(NULL, "Failed CreateCompatibleBitmap", "Error", MB_OK);
        return;
    }

    RenderFrame();
    PresentFrame();
}

void Renderer::SetPixel( int x, int y, COLORREF color, unsigned int depth, float fAlpha /*= 1.0f*/, float fSourceBlend )
{
    if(x > m_Image.width || x < 0 ||
       y >= m_Image.height || y <= 0)
    {
        return;
    }

    if(m_ausZBuffer[y*m_Width + x] > depth)
    {
        return;
    }

    

    BYTE *pPixel = NULL;

    //if(fAlpha == 1.0f)
    {
        m_ausZBuffer[y*m_Width + x] = depth;
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


                pPixel = &m_Image.pPixels[x*4+y*m_Image.width*4];
                pPixel[0] = (BYTE)(GetBValue(color) * fAlpha);// * (BYTE)((float)pPixel[3] / 255.0f);
                pPixel[1] = (BYTE)(GetGValue(color) * fAlpha);
                pPixel[2] = (BYTE)(GetRValue(color) * fAlpha);
                pPixel[3] = (BYTE)(255 * fAlpha);

        }
        else
        {
            /* Width of the image is not a multiple of 4. So padding bytes have
            * been included in the DIB's pixel data. Need to address each scan
            * line separately. This is much slower than the above case where the
            * width of the image is already a multiple of 4.
            */



                    pPixel = &m_Image.pPixels[(y * m_Image.pitch) + (x * 4)];
                    pPixel[0] = (BYTE)(GetBValue(color) * fAlpha);// * (BYTE)((float)pPixel[3] / 255.0f);
                    pPixel[1] = (BYTE)(GetGValue(color) * fAlpha);
                    pPixel[2] = (BYTE)(GetRValue(color) * fAlpha);
                    pPixel[3] = (BYTE)(255 * fAlpha);

        }
    }
//     else
//     {
//        
//     }
}

void Renderer::Rectangle( int left, int top, int right, int bottom, COLORREF color, unsigned int depth, float fAlpha /*= 1.0f*/ )
{
    for(int x = left; x <= right; x++)
    {
        for(int y = top; y <= bottom; y++)
        {
            SetPixel(x,y,color, depth, fAlpha);
        }
    }
}

void Renderer::HorizontalLine( int left, int right, int y, COLORREF color, unsigned int depth, float fAlpha /*= 1.0f*/ )
{
    for(int x = left; x <= right; x++)
    {
        SetPixel(x,y,color, depth, fAlpha);
    }
}

void Renderer::VerticalLine( int top, int bottom, int x, COLORREF color, unsigned int depth, float fAlpha /*= 1.0f*/ )
{
    for(int y = top; y <= bottom; y++)
    {
        SetPixel(x,y,color, depth, fAlpha);
    }
}

void Renderer::Circle( int xCenter, int yCenter, int radius, COLORREF c, unsigned int depth /*= 0*/, float fAlpha /*= 1.0f*/ )
{
    int x = 0;
    int y = radius;
    int p = (5 - radius*4)/4;

    circlePoints(xCenter, yCenter, x, y, c, depth, fAlpha);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        circlePoints(xCenter, yCenter, x, y, c, depth, fAlpha);
    }
}

void Renderer::circlePoints( int cx, int cy, int x, int y, COLORREF color, unsigned int depth, float alpha )
{
    if (x == 0) {
        SetPixel(cx, cy + y,color,depth,alpha);
        SetPixel(cx, cy - y,color,depth,alpha);
        SetPixel(cx + y, cy,color,depth,alpha);
        SetPixel(cx - y, cy,color,depth,alpha);
    } else if (x == y) {
        SetPixel(cx + x, cy + y,color,depth,alpha);
        SetPixel(cx - x, cy + y,color,depth,alpha);
        SetPixel(cx + x, cy - y,color,depth,alpha);
        SetPixel(cx - x, cy - y,color,depth,alpha);
    } else if (x < y) {
        SetPixel(cx + x, cy + y,color,depth,alpha);
        SetPixel(cx - x, cy + y,color,depth,alpha);
        SetPixel(cx + x, cy - y,color,depth,alpha);
        SetPixel(cx - x, cy - y,color,depth,alpha);
        SetPixel(cx + y, cy + x,color,depth,alpha);
        SetPixel(cx - y, cy + x,color,depth,alpha);
        SetPixel(cx + y, cy - x,color,depth,alpha);
        SetPixel(cx - y, cy - x,color,depth,alpha);
    }
}