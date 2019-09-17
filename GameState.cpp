#include "GameState.h"

GameState GAMESTATE;

void GameState::Brick::DrawBrick(HDC hdc)
{
    float fDarkerMod = .4f;
    float fBrighterMod = 1.6f;
    float fMyMod = 1.0f;
    float fAlpha = 1.0f;

    if(m_iRenderState == Brick::LRS_GLOW)
    {
        fDarkerMod *= 1.5f;
        fBrighterMod *= 1.5f;
        fMyMod *= 1.5f;

    }

    if(m_iRenderState == Brick::LRS_NO_ATTACH)
    {
        fAlpha = .5f;
        //fDarkerMod *= .5f;
        //fBrighterMod *= .5f;
        //fMyMod *= .5f;

    }

    COLORREF darker = RGB(GetRValue(m_wColor) * fDarkerMod,
                          GetGValue(m_wColor) * fDarkerMod,
                          GetBValue(m_wColor) * fDarkerMod);

    COLORREF brighter = m_wColor;
    float newR = GetRValue(m_wColor) * fBrighterMod;
    float newG = GetGValue(m_wColor) * fBrighterMod;
    float newB = GetBValue(m_wColor) * fBrighterMod;

    if( newR >= 256 )
    {
        newR = 255;
    }

    if( newG >= 256 )
    {
        newG = 255;
    }

    if( newB >= 256 )
    {
        newB = 255;
    }

    brighter = RGB(newR, newG, newB);

    COLORREF brightest = m_wColor;
    newR = GetRValue(m_wColor) * 2.25f;
    newG = GetGValue(m_wColor) * 2.25f;
    newB = GetBValue(m_wColor) * 2.25f;
    if( newR >= 256 )
    {
        newR = 255;
    }

    if( newG >= 256 )
    {
        newG = 255;
    }

    if( newB >= 256 )
    {
        newB = 255;
    }
    brightest = RGB(newR, newG, newB);


    COLORREF normal = m_wColor;
    newR = GetRValue(m_wColor) * fMyMod;
    newG = GetGValue(m_wColor) * fMyMod;
    newB = GetBValue(m_wColor) * fMyMod;
    if( newR >= 256 )
    {
        newR = 255;
    }

    if( newG >= 256 )
    {
        newG = 255;
    }

    if( newB >= 256 )
    {
        newB = 255;
    }
    normal = RGB(newR, newG, newB);



	unsigned int depth = m_iGroup * RENDER.GetWidth() * ((RENDER.GetHeight() - m_iRenderY) + RENDER.GetWidth() - m_iRenderX);
    RENDER.Rectangle(m_iRenderX, m_iRenderY, m_iRenderX + BRICK_WIDTH, m_iRenderY + BRICK_HEIGHT,normal, depth,fAlpha);

    


    for(int i = 0; i < BRICK_WIDTH/2; i++)
    {
        if( i+1 >= BRICK_WIDTH/2 )
        {
//             i++;
//             RENDER.VerticalLine(m_iRenderY-i, m_iRenderY+LEGO_HEIGHT-i,m_iRenderX-i, 0, depth,.5f);
//             RENDER.HorizontalLine(m_iRenderX-i,m_iRenderX+LEGO_WIDTH-i, m_iRenderY-i, 0, depth,.5f);

            RENDER.VerticalLine(m_iRenderY-i, m_iRenderY+BRICK_HEIGHT-i,m_iRenderX-i, 0, depth,fAlpha);
            RENDER.HorizontalLine(m_iRenderX-i,m_iRenderX+BRICK_WIDTH-i, m_iRenderY-i, 0, depth,fAlpha);
        }
        else
        {

            RENDER.VerticalLine(m_iRenderY-i, m_iRenderY+BRICK_HEIGHT-i,m_iRenderX-i,darker, depth,fAlpha);
            RENDER.HorizontalLine(m_iRenderX-i,m_iRenderX+BRICK_WIDTH-i, m_iRenderY-i, brighter, depth,fAlpha);
        }


        //RENDER.SetPixel(m_iRenderX-i,m_iRenderY-i,0, depth);
        RENDER.SetPixel(m_iRenderX-i,m_iRenderY+BRICK_HEIGHT-i,0, depth,fAlpha);
/*        RENDER.SetPixel(m_iRenderX-i-1,m_iRenderY+LEGO_HEIGHT-i,0, depth,.5f);*/

        if(m_poRight == NULL)
        {
            RENDER.SetPixel(m_iRenderX+BRICK_WIDTH-i,m_iRenderY-i,0, depth,fAlpha);
/*            RENDER.SetPixel(m_iRenderX+LEGO_WIDTH-i+1,m_iRenderY-i,0, depth,.5f);*/
        }
    }

    //RENDER.HorizontalLine(m_iRenderX,m_iRenderX+LEGO_WIDTH, m_iRenderY, 0, depth);
    RENDER.HorizontalLine(m_iRenderX,m_iRenderX+BRICK_WIDTH, m_iRenderY + BRICK_HEIGHT, 0, depth,fAlpha);
/*    RENDER.HorizontalLine(m_iRenderX,m_iRenderX+LEGO_WIDTH, m_iRenderY + LEGO_HEIGHT+1, 0, depth,.5f);*/

    if(m_poLeft == NULL)
    {
        //RENDER.VerticalLine(m_iRenderY, m_iRenderY+LEGO_HEIGHT, m_iRenderX, 0, depth);
    }

    if(m_poRight == NULL)
    {
        RENDER.VerticalLine(m_iRenderY, m_iRenderY+BRICK_HEIGHT, m_iRenderX+BRICK_WIDTH, 0, depth,fAlpha);
/*        RENDER.VerticalLine(m_iRenderY, m_iRenderY+LEGO_HEIGHT, m_iRenderX+LEGO_WIDTH+1, 0, depth, .5f);*/
    }

    if(m_poTop == NULL && m_iRenderState != LRS_NO_ATTACH)
    {
        // Draw Dot ////////////////////////////////////////////////////////
        //  Draw the dot thing that connects to other bricks              //

        //RENDER.Circle((m_iRenderX + LEGO_WIDTH * .5f)-2, m_iRenderY - LEGO_WIDTH/4.0f, LEGO_WIDTH * .3f, darker, depth);

        int start = (int)(m_iRenderX + BRICK_WIDTH * .15f);
        int end = (int)(m_iRenderX + BRICK_WIDTH * .85f);
        int mid = (int)(m_iRenderX + BRICK_WIDTH * .5f);

        int height = (int)(BRICK_HEIGHT * .25f);
        int radius = (int)(BRICK_WIDTH * .35f);

        for(int i = start; i <= end; i++)
        {
            int baseY = m_iRenderY - BRICK_WIDTH/4;
            int baseX = i - BRICK_WIDTH/4;
            
            float realHeight = (.5f*sqrt((float)(radius * radius) - (float)((i-mid)*(i-mid))));
            float chopped = realHeight - ((int)realHeight);
            baseY += (int)(realHeight);

            float PercentDone = (float)(i-start)/(float)(end-start);
            COLORREF myColor;

            if(i == start || i == end)
            {
                myColor = 0;
            }
            else if(PercentDone <= .5f)
            {
                PercentDone *= 2.0f;
                float newR = GetRValue(darker) * (1.0f - PercentDone) + GetRValue(normal) * PercentDone;
                float newG = GetGValue(darker) * (1.0f - PercentDone) + GetGValue(normal) * PercentDone;
                float newB = GetBValue(darker) * (1.0f - PercentDone) + GetBValue(normal) * PercentDone;  

                myColor = RGB(newR,newG,newB);
            }
            else if(PercentDone <= .75f)
            {
                PercentDone -= .5f;
                PercentDone *= 4.0f;
                float newR = GetRValue(normal) * (1.0f - PercentDone) + GetRValue(brightest) * PercentDone;
                float newG = GetGValue(normal) * (1.0f - PercentDone) + GetGValue(brightest) * PercentDone;
                float newB = GetBValue(normal) * (1.0f - PercentDone) + GetBValue(brightest) * PercentDone;   

                myColor = RGB(newR,newG,newB);
            }
            else
            {
                PercentDone -= .75f;
                PercentDone *= 4.0f;
                float newR = GetRValue(brightest) * (1.0f - PercentDone) + GetRValue(normal) * PercentDone;
                float newG = GetGValue(brightest) * (1.0f - PercentDone) + GetGValue(normal) * PercentDone;
                float newB = GetBValue(brightest) * (1.0f - PercentDone) + GetBValue(normal) * PercentDone; 

                myColor = RGB(newR,newG,newB);
            }



            RENDER.VerticalLine(baseY - height, baseY, baseX, myColor, depth);

            if(i != start && i != end)
            {
                RENDER.VerticalLine(baseY - 2*(baseY - (m_iRenderY - BRICK_WIDTH/4)) - height , baseY - height, baseX, brighter, depth);
            }

            float AA = 1.0f - abs(chopped);
            float newR = GetRValue(brighter) * (1.0f - AA) + GetRValue(myColor) * AA;
            float newG = GetGValue(brighter) * (1.0f - AA) + GetGValue(myColor) * AA;
            float newB = GetBValue(brighter) * (1.0f - AA) + GetBValue(myColor) * AA;
            COLORREF AACol = RGB(newR,newG,newB);

            if(i != start && i != end)
            {
                RENDER.SetPixel(baseX, baseY-height,AACol,depth,fAlpha);


            RENDER.SetPixel(baseX, baseY - 2*(baseY - (m_iRenderY - BRICK_WIDTH/4)) - height,RGB(newR*(1.0f-AA),newG*(1.0f-AA),newB*(1.0f-AA)) ,depth,fAlpha);
            RENDER.SetPixel(baseX, baseY - 2*(baseY - (m_iRenderY - BRICK_WIDTH/4)) - height-1,0 ,depth,(1.0f-AA)*fAlpha);
            }
            newR = GetRValue(myColor) * (1.0f - AA) + GetRValue(brighter) * AA;
            newG = GetGValue(myColor) * (1.0f - AA) + GetGValue(brighter) * AA;
            newB = GetBValue(myColor) * (1.0f - AA) + GetBValue(brighter) * AA;
            AACol = RGB(newR,newG,newB);
            if(i != start && i != end)
            {
                RENDER.SetPixel(baseX, baseY,AACol,depth,fAlpha);
            }


        }

        //RENDER.VerticalLine(m_iRenderY - LEGO_WIDTH/4 - height-1, m_iRenderY - LEGO_WIDTH/4 + 1, start - LEGO_WIDTH/4, 0, depth);
        //RENDER.VerticalLine(m_iRenderY - LEGO_WIDTH/4 - height-1, m_iRenderY - LEGO_WIDTH/4 + 1, end - LEGO_WIDTH/4, 0, depth);

    }


}

void GameState::Brick::ClearBrick(HDC hdc)
{

    HBRUSH NewBrush = CreateSolidBrush(CLEAR_COLOR);
    HPEN NewPen = CreatePen(PS_SOLID, 0, CLEAR_COLOR);

    SelectObject(hdc, NewBrush);
    SelectObject(hdc, NewPen);

    Rectangle(hdc, m_iRenderX, m_iRenderY, m_iRenderX + BRICK_WIDTH, m_iRenderY + BRICK_HEIGHT); 

    DeleteObject(NewBrush);
    DeleteObject(NewPen);
}

bool GameState::Brick::IntersectsPoint(int x, int y)
{
    if(x < m_iX)
    {
        return false;
    }
    if(x > m_iX + BRICK_WIDTH)
    {
        return false;
    }

    if(y > m_iY + BRICK_HEIGHT)
    {
        return false;
    }

    if(y < m_iY)
    {
        return false;
    }

    return true;
}

bool GameState::Brick::IntersectsBrick( Brick* poOther )
{
    if (IntersectsPoint(poOther->m_iX, poOther->m_iY))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX + BRICK_WIDTH, poOther->m_iY))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX, poOther->m_iY + BRICK_HEIGHT))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX + BRICK_WIDTH, poOther->m_iY + BRICK_HEIGHT))
    {
        return true;
    }

    return false;

}


bool GameState::Brick::IntersectsAnyBrick()
{
    SetMarks(1);

    bool ret = RecursiveIntersectsAnyBrick();
    SetMarks(0);

    return ret;
}

bool GameState::Brick::CanMateWithBrick( Brick* poOther )
{
    if( m_poBottom == NULL )
    {
        if( m_iY + BRICK_HEIGHT + (BRICK_MATE_LENGTH) > poOther->m_iY && 
            m_iY + BRICK_HEIGHT < poOther->m_iY &&
            m_iX > poOther->m_iX-BRICK_WIDTH/2.0f &&
            m_iX < poOther->m_iX+BRICK_WIDTH/2.0f )
        {
            return true;
        }
    }

    if( m_poTop == NULL )
    {
        if( m_iY - BRICK_MATE_LENGTH < poOther->m_iY + BRICK_HEIGHT && 
            m_iY > poOther->m_iY + BRICK_HEIGHT && 
            m_iX > poOther->m_iX-BRICK_WIDTH/2.0f &&
            m_iX < poOther->m_iX+BRICK_WIDTH/2.0f )
        {
            return true;
        }
    }

    return false;
}

GameState::Brick* GameState::Brick::FindMate()
{
    SetMarks(1);
    Brick* ret = RecursiveFindMate();
    SetMarks(1);

    if(ret == NULL)
    {
        SetMarks(0);
        ClearMates();
        SetMarks(0);
        return NULL;
    }

    ret->SetMarks(3);
    RecursiveMakeMatches();
    ret->SetMarks(0);
    SetMarks(0);

    if(GAMESTATE.m_poMovingMate->m_iY < GAMESTATE.m_poStaticMate->m_iY )
    {
        GAMESTATE.m_poMovingMate->SetBrickRenderPosition(GAMESTATE.m_poStaticMate->m_iX,
                                                        GAMESTATE.m_poStaticMate->m_iY 
                                                           - (int)(BRICK_HEIGHT + BRICK_MATE_LENGTH/4.0f));
    }
    else
    {
        GAMESTATE.m_poMovingMate->SetBrickRenderPosition(GAMESTATE.m_poStaticMate->m_iX, 
                                                        GAMESTATE.m_poStaticMate->m_iY + (int)(BRICK_HEIGHT + BRICK_MATE_LENGTH/4.0f));
    }
    return ret;
}

GameState::Brick* GameState::Brick::RecursiveFindMate()
{
    m_iMark = 2;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        if (GAMESTATE.m_vpoBricks[i]->m_iMark == 0 && CanMateWithBrick(GAMESTATE.m_vpoBricks[i]))
        {
            GAMESTATE.m_poMovingMate = this;
            GAMESTATE.m_poStaticMate = GAMESTATE.m_vpoBricks[i];

            return GAMESTATE.m_vpoBricks[i];
        }
    }

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 1 )
        {
            Brick *found = m_poLeft->RecursiveFindMate();

            if(found != NULL)
                return found;
        }
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 1 )
        {
            Brick *found = m_poRight->RecursiveFindMate();

            if(found != NULL)
                return found;
        }
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 1 )
        {
            Brick *found = m_poTop->RecursiveFindMate();

            if(found != NULL)
                return found;
        }
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 1 )
        {
            Brick *found = m_poBottom->RecursiveFindMate();

            if(found != NULL)
                return found;
        }
    }

    return NULL;
}

void GameState::Brick::RecursiveMakeMatches()
{
    m_iMark = 2;

    m_poTopDesiredMate = NULL;
    m_poBottomDesiredMate = NULL;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        if (GAMESTATE.m_vpoBricks[i]->m_iMark == 3 && CanMateWithBrick(GAMESTATE.m_vpoBricks[i]))
        {
            GAMESTATE.m_vpoBricks[i]->m_iMark = 4;
            if (GAMESTATE.m_vpoBricks[i]->m_iY < m_iY)
            {
                m_poTopDesiredMate = GAMESTATE.m_vpoBricks[i];
            }
            else
            {
                m_poBottomDesiredMate = GAMESTATE.m_vpoBricks[i];
            }
        }
    }

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 1 )
        {
            m_poLeft->RecursiveMakeMatches();
        }
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 1 )
        {
            m_poRight->RecursiveMakeMatches();
        }
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 1 )
        {
            m_poTop->RecursiveMakeMatches();
        }
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 1 )
        {
            m_poBottom->RecursiveMakeMatches();
        }
    }
}

void GameState::Brick::SetBrickPosition(int x, int y)
{
    RecursiveFixNeighbors(x, y);
    SetMarks(0);
}

void GameState::Brick::SetBrickRenderPosition( int x, int y )
{
    RecursiveFixRenderNeighbors(x, y);
    SetMarks(0);
}


void GameState::Brick::AttachLeft( Brick * poLeft, bool bMoveOther )
{
    m_poLeft = poLeft;
    if(bMoveOther)
        m_poLeft->SetBrickPosition(m_iX - BRICK_WIDTH, m_iY);
    if(bMoveOther)
        m_poLeft->SetBrickRenderPosition(m_iRenderX - BRICK_WIDTH, m_iRenderY);

    m_poLeft->m_poRight = this;
}

void GameState::Brick::AttachRight( Brick * poRight, bool bMoveOther )
{
    m_poRight = poRight;
    if(bMoveOther)
        m_poRight->SetBrickPosition(m_iX - BRICK_WIDTH, m_iY);
    if(bMoveOther)
        m_poRight->SetBrickRenderPosition(m_iRenderX - BRICK_WIDTH, m_iRenderY);

    m_poRight->m_poLeft = this;
}

void GameState::Brick::AttachTop( Brick * poTop, bool bMoveOther )
{
    m_poTop = poTop;
    if(bMoveOther)
        m_poTop->SetBrickPosition(m_iX, m_iY - BRICK_HEIGHT);

    m_poTop->m_poBottom = this;
}

void GameState::Brick::AttachBottom( Brick * poBottom, bool bMoveOther )
{
    m_poBottom = poBottom;

    if(bMoveOther)
        m_poBottom->SetBrickPosition(m_iX, m_iY + BRICK_HEIGHT);

    m_poBottom->m_poTop = this;
}

void GameState::Brick::SetMarks( int mark )
{
    if( m_iMark == mark )
    {
        return;
    }

    m_iMark = mark;
    
    if(m_poLeft != NULL && m_poLeft->m_iMark != mark)
    {
        m_poLeft->SetMarks(mark);
    }

    if(m_poRight != NULL && m_poRight->m_iMark != mark)
    {
        m_poRight->SetMarks(mark);
    }

    if(m_poTop != NULL && m_poTop->m_iMark != mark)
    {
        m_poTop->SetMarks(mark);
    }

    if(m_poBottom != NULL && m_poBottom->m_iMark != mark)
    {
        m_poBottom->SetMarks(mark);
    }
}

int GameState::Brick::ComputeDepth()
{
	return ((RENDER.GetHeight() - m_iRenderY) * RENDER.GetWidth() + RENDER.GetWidth() - m_iRenderX);
}

void GameState::Brick::RecursiveSetGroup(int group)
{
	if (m_iGroup != group)
	{
		m_iGroup = group;

		if (m_poLeft != NULL)
		{
			m_poLeft->RecursiveSetGroup(group);
		}

		if (m_poRight != NULL)
		{
			m_poRight->RecursiveSetGroup(group);
		}

		if (m_poTop != NULL)
		{
			m_poTop->RecursiveSetGroup(group);
		}

		if (m_poBottom != NULL)
		{
			m_poBottom->RecursiveSetGroup(group);
		}
	}
}


void GameState::Brick::RecursiveFixNeighbors(int x, int y)
{
    if(m_iMark != 0)
    {
		return;
    }

    m_iMark = 1;

    m_iX = x;
    m_iY = y;

    if(m_poLeft != NULL)
    {
        m_poLeft->RecursiveFixNeighbors(x - BRICK_WIDTH, y);
    }

    if(m_poRight != NULL)
    {
		m_poRight->RecursiveFixNeighbors(x + BRICK_WIDTH, y);
    }

    if(m_poTop != NULL)
    {
		m_poTop->RecursiveFixNeighbors(x, y - BRICK_HEIGHT);
    }

    if(m_poBottom != NULL)
    {
		m_poBottom->RecursiveFixNeighbors(x, y + BRICK_HEIGHT);
    }
}

void GameState::Brick::RecursiveFixRenderNeighbors(int x, int y)
{
    if(m_iMark != 0)
    {
		return;
    }

    m_iMark = 1;

    m_iRenderX = x;
    m_iRenderY = y;

    if(m_poLeft != NULL)
    {
        m_poLeft->RecursiveFixRenderNeighbors(x - BRICK_WIDTH, y);
    }

    if(m_poRight != NULL)
    {
        m_poRight->RecursiveFixRenderNeighbors(x + BRICK_WIDTH, y);
    }

    if(m_poTop != NULL)
    {
        m_poTop->RecursiveFixRenderNeighbors(x, y - BRICK_HEIGHT);
    }

    if(m_poBottom != NULL)
    {
        m_poBottom->RecursiveFixRenderNeighbors(x, y + BRICK_HEIGHT);
    }
}

bool GameState::Brick::RecursiveIntersectsAnyBrick()
{
    m_iMark = 2;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        if (GAMESTATE.m_vpoBricks[i]->m_iMark == 0 && GAMESTATE.m_vpoBricks[i]->IntersectsBrick(this))
        {
            return true;
        }
    }

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 1 && m_poLeft->RecursiveIntersectsAnyBrick())
            return true;
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 1 && m_poRight->RecursiveIntersectsAnyBrick())
            return true;
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 1 && m_poTop->RecursiveIntersectsAnyBrick())
            return true;
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 1 && m_poBottom->RecursiveIntersectsAnyBrick())
            return true;
    }

    return false;
}

void GameState::Brick::RecursiveMate()
{
    m_iMark = 1;

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 0)
            m_poLeft->RecursiveMate();
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 0)
            m_poRight->RecursiveMate();
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 0)
            m_poTop->RecursiveMate();
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 0)
            m_poBottom->RecursiveMate();
    }

    if(m_poTopDesiredMate)
    {
        m_poTop = m_poTopDesiredMate;
        m_poTop->m_poBottom = this;
        m_poTopDesiredMate = NULL;
    }

    if(m_poBottomDesiredMate)
    {
        m_poBottom = m_poBottomDesiredMate;
        m_poBottom->m_poTop = this;
        m_poBottomDesiredMate = NULL;
    }

}

void GameState::Brick::Mate()
{

    RecursiveClearDistances();
    SetMarks(0);

    RecursiveClearHoldWeights();
    SetMarks(0);

    RecursiveMate();
    SetMarks(0);

    GAMESTATE.m_poStaticMate->SetMarks(0);

    GAMESTATE.m_poStaticMate->RecursiveFixRenderNeighbors(GAMESTATE.m_poStaticMate->m_iX, GAMESTATE.m_poStaticMate->m_iY);
    SetMarks(0);

    GAMESTATE.m_poStaticMate->RecursiveFixNeighbors(GAMESTATE.m_poStaticMate->m_iX, GAMESTATE.m_poStaticMate->m_iY);
    SetMarks(0);

    ClearMates();
    SetMarks(0);

    GAMESTATE.PlayAttach();
}

void GameState::Brick::ClearMates()
{
    m_iMark = 1;

    m_poBottomDesiredMate = 
        m_poTopDesiredMate = NULL;

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 0)
            m_poLeft->ClearMates();
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 0)
            m_poRight->ClearMates();
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 0)
            m_poTop->ClearMates();
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 0)
            m_poBottom->ClearMates();
    }
}

bool GameState::Brick::CanDetachBrickUp()
{
    m_iMark = 1;
    bool bIsAttached = false;

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 1;
    }

    for( Brick* iter = m_poRight; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 1;
    }


    int iNewMark = 2;
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poBottom)
        {
            bIsAttached = true;
            iNewMark++;
            if(!iter->m_poBottom->RecursiveCanDetachBrickUp(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
        
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poBottom)
        {
            bIsAttached = true;
            iNewMark++;
            if(!iter->m_poBottom->RecursiveCanDetachBrickUp(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }


    SetMarks(0);
    return bIsAttached;
}

bool GameState::Brick::RecursiveCanDetachBrickUp(int mark)
{
    m_iMark = mark;


    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 1)
            return false;

        if(m_poBottom->m_iMark != mark)
        {
            if(!m_poBottom->RecursiveCanDetachBrickUp(mark))
            {
                return false;
            }
        }
    }

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 1)
            return true;

        if(m_poLeft->m_iMark != mark)
        {
            if(!m_poLeft->RecursiveCanDetachBrickUp(mark))
            {
                return false;
            }
        }
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 1)
            return true;

        if(m_poRight->m_iMark != mark)
        {
            if(!m_poRight->RecursiveCanDetachBrickUp(mark))
            {
                return false;
            }
        }
    }

    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 1)
            return true;

        if(m_poTop->m_iMark != mark)
        {
            if(!m_poTop->RecursiveCanDetachBrickUp(mark))
            {
                return false;
            }
        }
    }

    return true;
}

void GameState::Brick::DetachBrickUp()
{
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poBottom)
        {
            iter->m_poBottom->m_poTop = NULL;
            iter->m_poBottom = NULL;
        }
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poBottom)
        {
            iter->m_poBottom->m_poTop = NULL;
            iter->m_poBottom = NULL;
        }
    }

    GAMESTATE.PlayDetach();
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


bool GameState::Brick::CanDetachBrickDown()
{
    m_iMark = 1;
    bool bIsAttached = false;

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 1;
    }

    for( Brick* iter = m_poRight; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 1;
    }


    int iNewMark = 2;
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poTop)
        {
            bIsAttached = true;
            iNewMark++;
            if(!iter->m_poTop->RecursiveCanDetachBrickDown(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }

    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poTop)
        {
            bIsAttached = true;
            iNewMark++;
            if(!iter->m_poTop->RecursiveCanDetachBrickDown(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }


    SetMarks(0);
    return bIsAttached;
}

bool GameState::Brick::RecursiveCanDetachBrickDown(int mark)
{
    m_iMark = mark;


    if(m_poTop != NULL)
    {
        if(m_poTop->m_iMark == 1)
            return false;

        if(m_poTop->m_iMark != mark)
        {
            if(!m_poTop->RecursiveCanDetachBrickDown(mark))
            {
                return false;
            }
        }
    }

    if(m_poLeft != NULL)
    {
        if(m_poLeft->m_iMark == 1)
            return true;

        if(m_poLeft->m_iMark != mark)
        {
            if(!m_poLeft->RecursiveCanDetachBrickDown(mark))
            {
                return false;
            }
        }
    }

    if(m_poRight != NULL)
    {
        if(m_poRight->m_iMark == 1)
            return true;

        if(m_poRight->m_iMark != mark)
        {
            if(!m_poRight->RecursiveCanDetachBrickDown(mark))
            {
                return false;
            }
        }
    }

    if(m_poBottom != NULL)
    {
        if(m_poBottom->m_iMark == 1)
            return true;

        if(m_poBottom->m_iMark != mark)
        {
            if(!m_poBottom->RecursiveCanDetachBrickDown(mark))
            {
                return false;
            }
        }
    }

    return true;
}

void GameState::Brick::DetachBrickDown()
{
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poTop)
        {
            iter->m_poTop->m_poBottom = NULL;
            iter->m_poTop = NULL;
        }
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poTop)
        {
            iter->m_poTop->m_poBottom = NULL;
            iter->m_poTop = NULL;
        }
    }

    GAMESTATE.PlayDetach();
}

void GameState::Brick::CalculateDistances()
{
    return;

    RecursiveClearDistances();
    SetMarks(0);
    RecursiveCalculateDistances(0);
    SetMarks(0);
}

void GameState::Brick::RecursiveClearDistances()
{
    if(m_iMark == 1)
    {
        return;
    }

    m_iMark = 1;
    m_iDistance = -1;

    if(m_poLeft != NULL)
    {
        m_poLeft->RecursiveClearDistances();
    }

    if(m_poRight != NULL)
    {
        m_poRight->RecursiveClearDistances();
    }

    if(m_poTop != NULL)
    {
        m_poTop->RecursiveClearDistances();
    }

    if(m_poBottom != NULL)
    {
        m_poBottom->RecursiveClearDistances();
    }
}

void GameState::Brick::RecursiveCalculateDistances( int distance )
{
    if(m_iMark == 1)
    {
        return;
    }

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 1;
        iter->m_iDistance = distance;
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 1;
        iter->m_iDistance = distance;
    }


    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poTop)
        {
            iter->m_poTop->RecursiveCalculateDistances(distance+1);
        }

        if(iter->m_poBottom)
        {
            iter->m_poBottom->RecursiveCalculateDistances(distance+1);
        }
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poTop)
        {
            iter->m_poTop->RecursiveCalculateDistances(distance+1);
        }

        if(iter->m_poBottom)
        {
            iter->m_poBottom->RecursiveCalculateDistances(distance+1);
        }
    }

}

void GameState::Brick::CalculateWeights()
{ 
    RecursiveClearHoldWeights();
    SetMarks(0);


    CalculateDistances();



    float w = GetWeight();

    char buff[300];
    sprintf_s(buff, 300, "Total Weight: %f", w);

}


float GameState::Brick::GetWeight()
{
    if(m_fBlockWeight >= 0)
    {
        return m_fBlockWeight;
    }

    float fMyWeight = 0;

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_iDistance != m_iDistance)
        {
            MessageBox(NULL, "WRROR", "Error", MB_OK);
        }

        if(iter->m_fTopWeightHeld < 0)
        {
            if(iter->m_poTop && iter->m_poTop->m_iDistance > m_iDistance)
            {
                iter->m_fTopWeightHeld = iter->m_poTop->GetWeight()/ iter->m_poTop->GetNumConnections(iter); 
            }
            else
            {
                iter->m_fTopWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fTopWeightHeld;


        if(iter->m_fBottomWeightHeld < 0)
        {
            if(iter->m_poBottom && iter->m_poBottom->m_iDistance > m_iDistance)
            {
                iter->m_fBottomWeightHeld = iter->m_poBottom->GetWeight()/ iter->m_poBottom->GetNumConnections(iter); 
            }
            else
            {
                iter->m_fBottomWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fBottomWeightHeld;

        fMyWeight += 1.0f;
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_fTopWeightHeld < 0)
        {
            if(iter->m_poTop && iter->m_poTop->m_iDistance > m_iDistance)
            {
                iter->m_fTopWeightHeld = iter->m_poTop->GetWeight()/ iter->m_poTop->GetNumConnections(iter); 
            }
            else
            {
                iter->m_fTopWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fTopWeightHeld;


        if(iter->m_fBottomWeightHeld < 0)
        {
            if(iter->m_poBottom && iter->m_poBottom->m_iDistance > m_iDistance)
            {
                iter->m_fBottomWeightHeld = iter->m_poBottom->GetWeight()/ iter->m_poBottom->GetNumConnections(iter); 
            }
            else
            {
                iter->m_fBottomWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fBottomWeightHeld;

        fMyWeight += 1.0f;
    }



    // Set the block weight
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_fBlockWeight = fMyWeight;
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_fBlockWeight = fMyWeight;
    }

    return fMyWeight;

}

int GameState::Brick::GetNumConnections( Brick* poOther )
{
    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 1;
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 1;
    }


    int connections = 0;
    for( Brick* iter = poOther->m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poTop && iter->m_poTop->m_iMark == 1)
            connections++;

        if(iter->m_poBottom && iter->m_poBottom->m_iMark == 1)
            connections++;
    }

    for( Brick* iter = poOther; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poTop && iter->m_poTop->m_iMark == 1)
            connections++;
        
        if(iter->m_poBottom && iter->m_poBottom->m_iMark == 1)
            connections++;
    }

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 0;
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 0;
    }

    return connections;
}

void GameState::Brick::RecursiveClearHoldWeights()
{
    if(m_iMark != 1)
    {
        m_iMark = 1;
        m_fBottomWeightHeld = m_fTopWeightHeld = -1.0f;

        m_fBlockWeight = -1.0f;

        if(m_poLeft != NULL)
        {
            m_poLeft->RecursiveClearHoldWeights();
        }

        if(m_poRight != NULL)
        {
            m_poRight->RecursiveClearHoldWeights();
        }

        if(m_poTop != NULL)
        {
            m_poTop->RecursiveClearHoldWeights();
        }

        if(m_poBottom != NULL)
        {
            m_poBottom->RecursiveClearHoldWeights();
        }
    }
    
}

void GameState::Brick::BreakStressedConnections()
{
    float f = (float)rand()/(float)RAND_MAX;
    f *= 1000;
    f += 8;

    RecursiveBreakStressedConnections(f);
    SetMarks(0);

    CalculateWeights();
}

void GameState::Brick::RecursiveBreakStressedConnections(float fThreshold)
{
    if(m_iMark == 1)
    {
        return;
    }

    m_iMark = 1;

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        iter->m_iMark = 1;
        if(iter->m_fBottomWeightHeld > fThreshold)
        {
            if(iter->m_poBottom && iter->GetNumConnections(iter->m_poBottom) == 1)
            {
                iter->m_poBottom->m_poTop = NULL;
                iter->m_poBottom = NULL;
            }
        }

        if(iter->m_fTopWeightHeld > fThreshold)
        {
            if(iter->m_poTop && iter->GetNumConnections(iter->m_poTop) == 1)
            {
                iter->m_poTop->m_poBottom = NULL;
                iter->m_poTop = NULL;
            }
        }
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        iter->m_iMark = 1;
        if(iter->m_fBottomWeightHeld > fThreshold)
        {
            if(iter->m_poBottom && iter->GetNumConnections(iter->m_poBottom) == 1)
            {
                iter->m_poBottom->m_poTop = NULL;
                iter->m_poBottom = NULL;
            }
        }

        if(iter->m_fTopWeightHeld > fThreshold)
        {
            if(iter->m_poTop && iter->GetNumConnections(iter->m_poTop) == 1)
            {
                iter->m_poTop->m_poBottom = NULL;
                iter->m_poTop = NULL;
            }
        }
    }

    for( Brick* iter = m_poLeft; iter != NULL; iter = iter->m_poLeft )
    {
        if(iter->m_poBottom)
            iter->m_poBottom->RecursiveBreakStressedConnections(fThreshold);

        if(iter->m_poTop)
            iter->m_poTop->RecursiveBreakStressedConnections(fThreshold);
    }

    for( Brick* iter = this; iter != NULL; iter = iter->m_poRight )
    {
        if(iter->m_poBottom)
            iter->m_poBottom->RecursiveBreakStressedConnections(fThreshold);

        if(iter->m_poTop)
            iter->m_poTop->RecursiveBreakStressedConnections(fThreshold);
    }

}

void GameState::Brick::SetRenderState(int iNewState)
{
    RecursiveSetRenderState(iNewState);
    SetMarks(0);
    RENDER.RenderFrame();
    RENDER.PresentFrame();
}

void GameState::Brick::RecursiveSetRenderState(int iNewState )
{
    if(m_iMark != 1)
    {
        m_iMark = 1;
        m_iRenderState = iNewState;

        if(m_poLeft != NULL)
        {
            m_poLeft->RecursiveSetRenderState(iNewState);
        }

        if(m_poRight != NULL)
        {
            m_poRight->RecursiveSetRenderState(iNewState);
        }

        if(m_poTop != NULL)
        {
            m_poTop->RecursiveSetRenderState(iNewState);
        }

        if(m_poBottom != NULL)
        {
            m_poBottom->RecursiveSetRenderState(iNewState);
        }
    }
}

void GameState::GenerateBricks()
{
    for( int i = 0; i < 100; i++ )
    {
        COLORREF color = RGB(0, 0, 0);
        int colorNum = rand() % 5;
        if(colorNum == 0)
        {
            color = RGB(196,40,27);
        } 
        else if(colorNum == 1)
        {
            color = RGB(242,243,242);
        } 
        else if(colorNum == 2)
        {
            color = RGB(245,205,47);
        }
        else if(colorNum == 3)
        {
            color = RGB(13,105,171);
        }
        else if(colorNum == 4)
        {
            color = RGB(75,151,74);
        }

        int width = rand()%3;

        m_vpoBricks.push_back(new Brick(color, rand() % 1000, rand() % 768));
        
        for(int i = 0; i < pow(2.0f,width)- 1; i++)
        {
            Brick* newLego = new Brick(color, rand() % 1000, rand() % 768);
            m_vpoBricks.back()->AttachLeft(newLego);
            m_vpoBricks.push_back(newLego);
        }
    }
}

void GameState::Shutdown()
{

}

void GameState::Update()
{

}



void GameState::SetState(int iNewState)
{
    if (iNewState == GS_Static)
    {
        if (m_iState == GS_Drag_Mating)
        {
            m_poSelectedBrick->Mate();
            m_poSelectedBrick->SetMarks(0);
        }
        m_poSelectedBrick->SetRenderState(Brick::LRS_SOLID);
    }
    else if( iNewState == GS_Drag_Free )
    {

    }

        
    m_iState = iNewState;
}

int GameState::GetState()
{
    return m_iState;
}

void GameState::SetDragging(GameState::Brick* poDragged, int x, int y)
{
    m_poSelectedBrick = poDragged;
    m_iXOffset = m_poSelectedBrick->GetX() - x;
    m_iYOffset = m_poSelectedBrick->GetY() - y;
    SetState(GS_Drag_Free);
    m_poSelectedBrick->SetRenderState(Brick::LRS_GLOW);
    //m_poSelectedLego->CalculateWeights();
}

void GameState::SetSplitDragging(GameState::Brick* poDragged, int x, int y)
{
    m_poSelectedBrick = poDragged;
    m_iXOffset = m_poSelectedBrick->GetX() - x;
    m_iYOffset = m_poSelectedBrick->GetY() - y;
    SetState(GS_Drag_Splitting);
    m_poSelectedBrick->SetRenderState(Brick::LRS_NO_ATTACH);
    //m_poSelectedLego->CalculateWeights();
}

void  GameState::MoveSelected(int x, int y)
{

    m_poSelectedBrick->SetBrickPosition(m_iXOffset + x, m_iYOffset + y);
    //m_poSelectedLego->BreakStressedConnections();

    float delta = sqrt((float)((x - m_iLastX) * (x - m_iLastX) + 
                               (y - m_iLastY) * (y - m_iLastY)));

    m_iLastX = x;
    m_iLastY = y;

    if(false)
    {
        if(m_iState == GS_Drag_Mating && m_poSelectedBrick->IntersectsAnyBrick())
        {
            m_poSelectedBrick->Mate();
            m_iState = GS_Static;
        }
        else
        {
            SetState(GS_Drag_Free);
            m_poSelectedBrick->SetBrickRenderPosition(m_iXOffset + x, m_iYOffset +  y);
        }
        return;
    }

    if(m_iState != GS_Drag_Splitting)
    {
        if(m_poSelectedBrick->IntersectsAnyBrick())
        {
            if(m_iState == GS_Drag_Mating)
            {
                m_poSelectedBrick->Mate();
                m_iState = GS_Static;
            }
            else
            {
                m_poSelectedBrick->SetBrickRenderPosition(m_iXOffset + x, m_iYOffset +  y);
            }
        }
        else
        {
            Brick *mate = m_poSelectedBrick->FindMate();
            if(mate)
            {
                SetState(GS_Drag_Mating);
            }
            else
            {
                SetState(GS_Drag_Free);
                m_poSelectedBrick->SetBrickRenderPosition(m_iXOffset + x, m_iYOffset +  y);
            }
        }
    }
    else
    {
        m_poSelectedBrick->SetBrickRenderPosition(m_iXOffset + x, m_iYOffset +  y);
    }
}

GameState::Brick* GameState::GetBrickAt(int x, int y)
{
    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        if (GAMESTATE.m_vpoBricks[i]->IntersectsPoint(x,y))
        {
            return GAMESTATE.m_vpoBricks[i];
        }
    }
    
    return NULL;
}

void GameState::PlayAttach()
{
    char buff[255];
    sprintf_s(buff,255,"attach%i", rand()%4);
    gPlaySound(buff);
}


void GameState::PlayDetach()
{
    char buff[255];
    sprintf_s(buff,255,"detach%i", rand()%4);
    gPlaySound(buff);
}

void GameState::DumpGamestate( char * cstrName )
{

    std::ofstream out;

    out.open(cstrName, std::ofstream::out );


    std::map<const Brick*, int> indexMap;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        indexMap[m_vpoBricks[i]] = i;
        out << i << " " 
            << m_vpoBricks[i]->GetX() << " " 
            << m_vpoBricks[i]->GetY() << " "
            << m_vpoBricks[i]->GetColor() << " "
            << std::endl;
    }

    out << "-1" << std::endl;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBricks.size(); i++ )
    {
        out << i << " ";

        if(m_vpoBricks[i]->HasLeft())
        {
            out << indexMap[m_vpoBricks[i]->GetLeft()] << " ";
        }
        else
        {
            out << -1 << " ";
        }

        if(m_vpoBricks[i]->HasTop())
        {
            out << indexMap[m_vpoBricks[i]->GetTop()] << " ";
        }
        else
        {
            out << -1 << " ";
        }


        if(m_vpoBricks[i]->HasRight())
        {
            out << indexMap[m_vpoBricks[i]->GetRight()] << " ";
        }
        else
        {
            out << -1 << " ";
        }


        if(m_vpoBricks[i]->HasBottom())
        {
            out << indexMap[m_vpoBricks[i]->GetBottom()] << std::endl;
        }
        else
        {
            out << -1 << std::endl;
        }

    }

    out.close();
}


bool GameState::ReadGamestate( char * cstrName )
{

    std::ifstream in;

    in.open(cstrName, std::ofstream::in );

    if(!in.is_open())
    {
        return false;
    }

    for( unsigned int ui = 0; ui < GAMESTATE.m_vpoBricks.size(); ui++ )
    {
        delete m_vpoBricks[ui];
        m_vpoBricks[ui] = 0;
    }

    m_vpoBricks.clear();

    int i, x, y;
    COLORREF color;

    in >> i;
    while(i >= 0)
    {
        in >> x >> y >> color;
        Brick* temp = new Brick(color,x,y);

        m_vpoBricks.push_back(temp);

        in >> i;
    }

    

    int left, right, top, bottom;

    while(in >> i >> left >> top >> right >> bottom)
    {
        if(left >= 0)
        {
            m_vpoBricks[i]->AttachLeft(m_vpoBricks[left], false);
        }

        if(top >= 0)
        {
            m_vpoBricks[i]->AttachTop(m_vpoBricks[top], false);
        }

        if(bottom >= 0)
        {
            m_vpoBricks[i]->AttachBottom(m_vpoBricks[bottom], false);
        }

        if(right >= 0)
        {
            m_vpoBricks[i]->AttachRight(m_vpoBricks[right], false);
        }
    }
    in.close();
    return true;

}