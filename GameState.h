#ifndef __GameState_H__
#define __GameState_H__

#include "Blokaroka.h"
#include <map>

#define BRICK_WIDTH 15
#define BRICK_HEIGHT 18

#define BRICK_MATE_LENGTH 15

#define COLOR_WHITE			RGB(242,243,242)
#define COLOR_RED			RGB(196,40,27)
#define COLOR_YELLOW		RGB(245,205,47)
#define COLOR_GREEN			RGB(75,151,74)
#define COLOR_BLUE			RGB(13,105,171)

#define COLOR_AMETHYST_0	RGB(254,233,243)
#define COLOR_AMETHYST_1	RGB(238,166,201)
#define COLOR_AMETHYST_2	RGB(214,114,151)
#define COLOR_AMETHYST_3	RGB(170,90,127)

#define COLOR_TOPAZ_0		RGB(190,229,227)
#define COLOR_TOPAZ_1		RGB(118,206,215)
#define COLOR_TOPAZ_2		RGB(60,186,218)
#define COLOR_TOPAZ_3		RGB(45,133,189)

#define COLOR_PERIDOT_0		RGB(199,251,155)
#define COLOR_PERIDOT_1		RGB(145,217,85)
#define COLOR_PERIDOT_2		RGB(102,178,49)
#define COLOR_PERIDOT_3		RGB(65,131,28)


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

    class Brick 
    {
    public:
        Brick(COLORREF color, int x, int y) :
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
			m_iRenderState(LRS_SOLID),
			m_fBlockWeight(0),
			m_poTopDesiredMate(NULL),
			m_poBottomDesiredMate(NULL),
			m_iGroup(0)
        { };


        COLORREF m_wColor;

		int GetColorID();

        void ClearBrick(HDC hdc);
        void DrawBrick(HDC hdc);
        bool IntersectsPoint(int x, int y);
        bool IntersectsBrick(Brick* poOther);
        bool IntersectsAnyBrick();

        bool CanMateWithBrick(Brick* poOther);
        Brick* FindMate();
        void Mate();

        bool CanDetachBrickUp();
        void DetachBrickUp();

        bool CanDetachBrickDown();
        void DetachBrickDown();
       

        void SetBrickPosition(int x, int y);
        void SetBrickRenderPosition(int x, int y);

        void AttachLeft(Brick * poLeft, bool bMoveOther = true);
        void AttachRight(Brick * poRight, bool bMoveOther = true);

        void AttachTop(Brick * poTop, bool bMoveOther = true);
        void AttachBottom(Brick * poBottom, bool bMoveOther = true);

        void CalculateDistances();
        void CalculateWeights();


        void BreakStressedConnections();

        int GetX() const { return m_iX; };
        int GetY() const { return m_iY; };
        COLORREF GetColor() const {return m_wColor;};

        const Brick* GetLeft() const {return m_poLeft;};
        const Brick* GetRight() const {return m_poRight;};
        const Brick* GetTop() const {return m_poTop;};
        const Brick* GetBottom() const {return m_poBottom;};

        bool HasLeft() const {return m_poLeft != NULL;};
        bool HasRight() const {return m_poRight != NULL;};
        bool HasTop() const {return m_poTop != NULL;};
        bool HasBottom() const {return m_poBottom != NULL;};

        // Recursively set the mark of all neighbors.
        void SetMarks(int mark);

        void SetRenderState(int iNewState);

		int ComputeDepth();

		int m_iGroup;
		void RecursiveSetGroup(int group);

		void SetColorForMenu(int menu);

        enum
        {
            LRS_SOLID,
            LRS_GLOW,
            LRS_NO_ATTACH
        };

		int GetRenderState() { return m_iRenderState; };

    private:

        void RecursiveFixNeighbors(int x, int y);
        bool RecursiveIntersectsAnyBrick();
        void ClearMates();
        GameState::Brick* RecursiveFindMate();
        void RecursiveMakeMatches();
        void RecursiveMate();
        void RecursiveFixRenderNeighbors(int x, int y);

        bool RecursiveCanDetachBrickUp(int mark);
        bool RecursiveCanDetachBrickDown(int mark);

        void RecursiveClearHoldWeights();

        void RecursiveClearDistances();
        void RecursiveCalculateDistances(int distance=0);
        float GetWeight();

        int GetNumConnections(Brick* poOther);
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

        Brick * m_poLeft;
        Brick * m_poRight;
        Brick * m_poTop;
        Brick * m_poBottom;

        Brick * m_poTopDesiredMate;
        Brick * m_poBottomDesiredMate;
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
		m_poSelectedBrick(NULL),
		m_vpoBricks(NULL)
	{

	}

    void GenerateBricks();
    void Shutdown();
    void Update();

    void SetState(int iNewState);
    int GetState();

    void SetDragging(Brick* poDragged, int x, int y);

    void MoveSelected(int x, int y);

    Brick* GetBrickAt(int x, int y);

    void PlayAttach();
    void PlayDetach();

    void DumpGamestate(std::wstring cstrName);
    bool ReadGamestate(std::wstring cstrName);
    void SetSplitDragging(GameState::Brick* poDragged, int x, int y);

	void SetColorsForMenu(int menu);

    int m_iXOffset, m_iYOffset;

    std::vector<Brick*> m_vpoBricks;

    Brick * m_poMovingMate;
    Brick * m_poStaticMate;

private:

    Brick * m_poSelectedBrick;
    int m_iState;

    int m_iLastX;
    int m_iLastY;

    int m_iAttach1;




};

extern GameState GAMESTATE;

#endif // __GameState_H__