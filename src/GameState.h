#ifndef __GameState_H__
#define __GameState_H__

#include "Blokaroka.h"
#include <map>

#define BLOK_WIDTH 15
#define BLOK_HEIGHT 18

#define BLOK_MATE_LENGTH 15

#define COLOR_WHITE         RGB(242,243,242)
#define COLOR_RED           RGB(196,40,27)
#define COLOR_YELLOW        RGB(245,205,47)
#define COLOR_GREEN         RGB(75,151,74)
#define COLOR_BLUE          RGB(13,105,171)

#define COLOR_AMETHYST_0    RGB(254,233,243)
#define COLOR_AMETHYST_1    RGB(238,166,201)
#define COLOR_AMETHYST_2    RGB(214,114,151)
#define COLOR_AMETHYST_3    RGB(170,90,127)

#define COLOR_TOPAZ_0       RGB(190,229,227)
#define COLOR_TOPAZ_1       RGB(118,206,215)
#define COLOR_TOPAZ_2       RGB(60,186,218)
#define COLOR_TOPAZ_3       RGB(45,133,189)

#define COLOR_PERIDOT_0     RGB(199,251,155)
#define COLOR_PERIDOT_1     RGB(145,217,85)
#define COLOR_PERIDOT_2     RGB(102,178,49)
#define COLOR_PERIDOT_3     RGB(65,131,28)


class GameState
{
public:
    enum 
    {
        GS_Static,
        GS_Drag_Free,
        GS_Drag_Mating,
        GS_Drag_Splitting
    };

    

    GameState() :
        m_iAttach1(0),
        m_iLastX(0),
        m_iLastY(0),
        m_iState(0),
        m_iYOffset(0),
        m_iXOffset(0),
        m_poMovingMate(NULL),
        m_poStaticMate(NULL),
        m_poSelectedBlok(NULL),
        m_vpoBloks(NULL)
    {

    }

    void GenerateBloks();
    void Shutdown();
    void Update();
    void ClearBloks();
    void RegenPhysics();

    void SetState(int iNewState);
    int GetState();

    void SetDragging(Blok* poDragged, int x, int y);

    void MoveSelected(int x, int y);

    Blok* GetBlokAt(int x, int y);

    void PlayAttach();
    void PlayDetach();

    void DumpGamestate(std::wstring cstrName);
    bool ReadGamestate(std::wstring cstrName);
    void SetSplitDragging(Blok* poDragged, int x, int y);

    void SetColorsForMenu(int menu);

    int m_iXOffset, m_iYOffset;

    std::vector<Blok*> m_vpoBloks;

    Blok * m_poMovingMate;
    Blok * m_poStaticMate;

private:

    Blok * m_poSelectedBlok;
    int m_iState;

    int m_iLastX;
    int m_iLastY;

    int m_iAttach1;
};

extern GameState GAMESTATE;

#endif // __GameState_H__