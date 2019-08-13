#ifndef __GameState_H__
#define __GameState_H__

#include "LEGO.h"
#include <map>

#define LEGO_WIDTH 15
#define LEGO_HEIGHT 18

#define LEGO_MATE_LENGTH 15

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

    class Lego 
    {
    public:
        Lego(COLORREF color, int x, int y) :
          m_wColor(color),
          m_iX(x),
          m_iY(y),
          m_iRenderX(x),
          m_iRenderY(y),
          m_iMark(0),
          m_poLeft(NULL),
          m_poRight(NULL),
          m_poTop(NULL),
          m_poBottom(NULL),
          m_fBottomWeightHeld(-1.0f),
          m_fTopWeightHeld(-1.0f),
          m_iDistance(-1),
          m_iRenderState(LRS_SOLID)

        { };


        COLORREF m_wColor;

        void ClearLego(HDC hdc);
        void DrawLego(HDC hdc);
        bool IntersectsPoint(int x, int y);
        bool IntersectsLego(Lego* poOther);
        bool IntersectsAnyLego();

        bool CanMateWithLego(Lego* poOther);
        Lego* FindMate();
        void Mate();

        bool CanDetachLegoUp();
        void DetachLegoUp();

        bool CanDetachLegoDown();
        void DetachLegoDown();
       

        void SetLegoPosition(int x, int y);
        void SetLegoRenderPosition(int x, int y);

        void AttachLeft(Lego * poLeft, bool bMoveOther = true);
        void AttachRight(Lego * poRight, bool bMoveOther = true);

        void AttachTop(Lego * poTop, bool bMoveOther = true);
        void AttachBottom(Lego * poBottom, bool bMoveOther = true);

        void CalculateDistances();
        void CalculateWeights();


        void BreakStressedConnections();

        int GetX() const { return m_iX; };
        int GetY() const { return m_iY; };
        COLORREF GetColor() const {return m_wColor;};

        const Lego* GetLeft() const {return m_poLeft;};
        const Lego* GetRight() const {return m_poRight;};
        const Lego* GetTop() const {return m_poTop;};
        const Lego* GetBottom() const {return m_poBottom;};

        bool HasLeft() const {return m_poLeft != NULL;};
        bool HasRight() const {return m_poRight != NULL;};
        bool HasTop() const {return m_poTop != NULL;};
        bool HasBottom() const {return m_poBottom != NULL;};

        // Recursively set the mark of all neighbors.
        void SetMarks(int mark);

        void SetRenderState(int iNewState);

        enum
        {
            LRS_SOLID,
            LRS_GLOW,
            LRS_NO_ATTACH
        };

    private:

        void RecursiveFixNeighbors(int x, int y);
        bool RecursiveIntersectsAnyLego();
        void ClearMates();
        GameState::Lego* RecursiveFindMate();
        void RecursiveMakeMatches();
        void RecursiveMate();
        void RecursiveFixRenderNeighbors(int x, int y);

        bool RecursiveCanDetachLegoUp(int mark);
        bool RecursiveCanDetachLegoDown(int mark);

        void RecursiveClearHoldWeights();

        void RecursiveClearDistances();
        void RecursiveCalculateDistances(int distance=0);
        float GetWeight();

        int GetNumConnections(Lego* poOther);
        void RecursiveBreakStressedConnections(float fThreshold);

        void RecursiveSetRenderState(int iNewState);

        int m_iX, m_iY;
        int m_iRenderX, m_iRenderY;
        int m_iMark;

        int m_iRenderState;



        int m_iDistance;

        float m_fTopWeightHeld;
        float m_fBottomWeightHeld;
        float m_fBlockWeight;

        Lego * m_poLeft;
        Lego * m_poRight;
        Lego * m_poTop;
        Lego * m_poBottom;

        Lego * m_poTopDesiredMate;
        Lego * m_poBottomDesiredMate;
    };

    GameState();

    void GenerateLegos();
    void Shutdown();
    void Update();

    void SetState(int iNewState);
    int GetState();

    void SetDragging(Lego* poDragged, int x, int y);

    void MoveSelected(int x, int y);

    Lego* GetLegoAt(int x, int y);

    void PlayAttach();
    void PlayDetach();

    void DumpGamestate(char * cstrName);
    bool ReadGamestate(char * cstrName);
    void SetSplitDragging(GameState::Lego* poDragged, int x, int y);


    int m_iXOffset, m_iYOffset;

    std::vector<Lego*> m_vpoLegos;

    Lego * m_poMovingMate;
    Lego * m_poStaticMate;

private:

    Lego * m_poSelectedLego;
    int m_iState;

    int m_iLastX;
    int m_iLastY;

    int m_iAttach1;




};

extern GameState GAMESTATE;

#endif // __GameState_H__