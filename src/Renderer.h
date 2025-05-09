/**
 * @file Renderer.h
 * @brief This file contains the definition of the Renderer singleton.
 *
 * The renderer handles draw functions and windows messages from the window.
 * It is responsible for drawing the gamestate to the screen.
 *
 * The Renderer is a singleton that can be accessed with RENDER. There
 * should be only one Renderer in existence ever.
 */

#ifndef __RENDERER_H__
#define __RENDERER_H__

#define CLEAR_COLOR RGB(255, 0, 255)
#include "Blokaroka.h"

const UINT WM_TRAY = WM_USER + 1;

/* Generic wrapper around a DIB with a 32-bit color depth. */
typedef struct
{
    int width;
    int height;
    int pitch;
    HDC hdc;
    HBITMAP hBitmap;
    BITMAPINFO info;
    BYTE *pPixels;
} Image;

class Renderer
{
  public:
    class Pixel
    {
      public:
        Pixel() : m_iX(0), m_iY(0)
        {
        }

        Pixel(int x, int y)
        {
            m_iX = x;
            m_iY = y;
        };

        Pixel(b2Vec2 physicsPoint);

        void SetFromPhysics(b2Vec2 physicsPoint);

        b2Vec2 ToPhysics();

        int m_iX, m_iY;
    };

    Renderer()
        : hWnd(NULL), m_HDC(NULL), m_bRight(false), m_Width(0), m_Height(0),
          m_ausZBuffer(NULL), m_iLastMouseX(0), m_iLastMouseY(0),
          m_fLastMessageTime(-100.0f)
    {
        m_lastMessage[0] = '\0';
    };

    void InitRenderer(HINSTANCE hInstance);

    void Shutdown();

    void SetPixel(int x, int y, COLORREF color, unsigned int depth = 0,
                  float fAlpha = 1.0f, float fSourceBlend = 1.0f);
    void Rectangle(int left, int top, int right, int bottom, COLORREF color,
                   unsigned int depth = 0, float fAlpha = 1.0f);

    void HorizontalLine(int left, int right, int y, COLORREF color,
                        unsigned int depth = 0, float fAlpha = 1.0f);
    void VerticalLine(int top, int bottom, int x, COLORREF color,
                      unsigned int depth = 0, float fAlpha = 1.0f);

    void Line(Pixel start, Pixel end, COLORREF color, unsigned int depth,
              float alpha);
    void Line(int startX, int startY, int endX, int endY, COLORREF color,
              unsigned int depth, float alpha);

    void Circle(int x, int y, int radius, COLORREF c, unsigned int depth = 0,
                float fAlpha = 1.0f);

    void RenderFrame();
    void PresentFrame();
    void HandleWindows();

    void ShowInfoMessage(const char *format, ...);

    int GetHeight() const
    {
        return m_Height;
    };
    int GetWidth() const
    {
        return m_Width;
    };

    int m_iThing;

    // Windows message handling and instance handles.
    HINSTANCE m_hInstance;
    HWND hWnd;
    HDC m_HDC;
    NOTIFYICONDATA m_tnd;
    HMENU m_hPopupMenu;

    bool m_bRight;

    int m_iLastMouseX; // Use to calculate how much the mouse moved this
                       // frame (X)
    int m_iLastMouseY; // Use to calculate how much the mouse moved this
                       // frame (Y)

    bool m_bNeedsPop;

  private:
    int m_Width;  // Size of the window.
    int m_Height; // Size of the window.

    Image m_Image;

    // With the ZBuffer we can handle blocks that overlap more correctly.
    unsigned int *m_ausZBuffer;

    // Helper function for drawing circles.
    void circlePoints(int cx, int cy, int x, int y, COLORREF color,
                      unsigned int depth, float alpha);

    char
        m_lastMessage[1024]; // Used to see if we are spamming the same message.
    float m_fLastMessageTime; // Used to see how long the last message was sent.
};

inline void ImageDestroy(Image *pImage)
{
    if (!pImage)
        return;

    pImage->width = 0;
    pImage->height = 0;
    pImage->pitch = 0;

    if (pImage->hBitmap)
    {
        DeleteObject(pImage->hBitmap);
        pImage->hBitmap = NULL;
    }

    if (pImage->hdc)
    {
        DeleteDC(pImage->hdc);
        pImage->hdc = NULL;
    }

    memset(&pImage->info, 0, sizeof(pImage->info));
    pImage->pPixels = NULL;
}

inline BOOL ImageCreate(Image *pImage, int width, int height)
{
    /* All Windows DIBs are aligned to 4-byte (DWORD) memory boundaries. This
     * means that each scan line is padded with extra bytes to ensure that the
     * next scan line starts on a 4-byte memory boundary. The 'pitch' member
     * of the Image structure contains width of each scan line (in bytes).
     */

    if (!pImage)
        return FALSE;

    pImage->width = width;
    pImage->height = height;
    pImage->pitch = ((width * 32 + 31) & ~31) >> 3;
    pImage->pPixels = NULL;
    pImage->hdc = CreateCompatibleDC(NULL);

    if (!pImage->hdc)
        return FALSE;

    memset(&pImage->info, 0, sizeof(pImage->info));

    pImage->info.bmiHeader.biSize = sizeof(pImage->info.bmiHeader);
    pImage->info.bmiHeader.biBitCount = 32;
    pImage->info.bmiHeader.biWidth = width;
    pImage->info.bmiHeader.biHeight = -height;
    pImage->info.bmiHeader.biCompression = BI_RGB;
    pImage->info.bmiHeader.biPlanes = 1;

    pImage->hBitmap =
        CreateDIBSection(pImage->hdc, &pImage->info, DIB_RGB_COLORS,
                         (void **)&pImage->pPixels, NULL, 0);

    if (!pImage->hBitmap)
    {
        ImageDestroy(pImage);
        return FALSE;
    }

    GdiFlush();
    return TRUE;
}

extern Renderer RENDER;

#endif // __RENDERER_H__
