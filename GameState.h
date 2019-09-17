#ifndef __GameState_H__
#define __GameState_H__

#include "DesktopBricks.h"
#include <map>

#define BRICK_WIDTH 15
#define BRICK_HEIGHT 18

#define BRICK_MATE_LENGTH 15

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

    void DumpGamestate(char * cstrName);
    bool ReadGamestate(char * cstrName);
    void SetSplitDragging(GameState::Brick* poDragged, int x, int y);


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