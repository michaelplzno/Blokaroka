#ifndef __RENDERER_H__
#define __RENDERER_H__

#define CLEAR_COLOR RGB(255,0,255)
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
    Renderer() :
        hWnd(NULL),
        m_HDC(NULL),
        m_bRight(false),
        m_Width(0),
        m_Height(0),
        m_ausZBuffer(NULL)
    {
        // init singleton
    }

    void InitRenderer(HINSTANCE hInstance);

    void Shutdown();

    void SetPixel(int x, int y, COLORREF color, unsigned int depth = 0,float fAlpha = 1.0f, float fSourceBlend = 1.0f);
    void Rectangle(int left, int top, int right, int bottom, COLORREF color, unsigned int depth = 0, float fAlpha = 1.0f);

    void HorizontalLine(int left, int right, int y, COLORREF color, unsigned int depth = 0, float fAlpha = 1.0f);
    void VerticalLine(int top, int bottom, int x, COLORREF color, unsigned int depth = 0, float fAlpha = 1.0f);

    void Circle(int x, int y, int radius, COLORREF c, unsigned int depth = 0, float fAlpha = 1.0f);

    void RenderFrame();
    void PresentFrame();
    void HandleWindows();
    
    int GetHeight() const {return m_Height;};
    int GetWidth() const {return m_Width;};

    int m_iThing;

    HINSTANCE m_hInstance;
    HWND hWnd;
    HDC m_HDC;
    NOTIFYICONDATA m_tnd;
    HMENU m_hPopupMenu;
    
    bool m_bRight;
  private:

      int m_Width;
      int m_Height;
      Image m_Image;

      unsigned int* m_ausZBuffer;


      void circlePoints(int cx, int cy, int x, int y, COLORREF color, unsigned int depth, float alpha);
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

    pImage->hBitmap = CreateDIBSection(pImage->hdc, &pImage->info,
        DIB_RGB_COLORS, (void**)&pImage->pPixels, NULL, 0);

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
