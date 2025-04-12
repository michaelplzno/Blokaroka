#include "Blok.h"

Blok::~Blok()
{
    /* if (m_poLeft == NULL && m_poPhysicsBody != NULL)
     {
         PHYSICS.DestroyBody(m_poPhysicsBody);
     }*/
}

void Blok::Group(std::map<Blok*, bool>& used) 
{
    used[this] = 1; // mark, that we were here
    int offset = BLOK_WIDTH - m_width;

    //check if there top blok and if we weren't here
    if(HasTop() && !used[m_poTop]){
        m_poTop->m_iRenderX = m_iRenderX;
        m_poTop->m_iRenderY = m_iRenderY - m_height;
        m_poTop->Group(used);
    }

    if(HasLeft() && !used[m_poLeft]){
        m_poLeft->m_iRenderX = m_iRenderX - m_width;
        m_poLeft->m_iRenderY = m_iRenderY;
        m_poLeft->Group(used);
    }

    if(HasBottom() && !used[m_poBottom]){
        m_poBottom->m_iRenderX = m_iRenderX;
        m_poBottom->m_iRenderY = m_iRenderY + m_height;
        m_poBottom->Group(used);
    }

    if(HasRight() && !used[m_poRight]){
        m_poRight->m_iRenderX = m_iRenderX + m_width;
        m_poRight->m_iRenderY = m_iRenderY;
        m_poRight->Group(used);
    }
}
void subtractAllOffsets()
{
    std::map<Blok*, bool> used;//map for marking bloks where we already were
    for(int i = 0; i < GAMESTATE.m_vpoBloks.size(); ++i)
        if(!used[GAMESTATE.m_vpoBloks[i]])GAMESTATE.m_vpoBloks[i]->Group(used);
}


int Blok::GetWidth()
{
    return m_width;
}
int Blok::GetHeight()
{
    return m_height;
}
void Blok::SetWidth(int newWidth)
{
    m_width = newWidth;
}
void Blok::SetHeight(int newHeight)
{
    m_height = newHeight;
}

int Blok::GetColorID()
{
    if (m_wColor == COLOR_WHITE)
    {
        return 0;
    }
    else if (m_wColor == COLOR_RED || m_wColor == COLOR_AMETHYST_0 ||
             m_wColor == COLOR_TOPAZ_0 || m_wColor == COLOR_PERIDOT_0)
    {
        return 1;
    }
    else if (m_wColor == COLOR_YELLOW || m_wColor == COLOR_AMETHYST_1 ||
             m_wColor == COLOR_TOPAZ_1 || m_wColor == COLOR_PERIDOT_1)
    {
        return 2;
    }
    else if (m_wColor == COLOR_GREEN || m_wColor == COLOR_AMETHYST_2 ||
             m_wColor == COLOR_TOPAZ_2 || m_wColor == COLOR_PERIDOT_2)
    {
        return 3;
    }
    else if (m_wColor == COLOR_BLUE || m_wColor == COLOR_AMETHYST_3 ||
             m_wColor == COLOR_TOPAZ_3 || m_wColor == COLOR_PERIDOT_3)
    {
        return 4;
    }

    return -1;
}

void Blok::DrawBlok(HDC hdc)
{
    float fDarkerMod = .4f;
    float fBrighterMod = 1.6f;
    float fMyMod = 1.0f;
    float fAlpha = 1.0f;

    if (m_iRenderState == Blok::BRS_GLOW)
    {
        fDarkerMod *= 1.5f;
        fBrighterMod *= 1.5f;
        fMyMod *= 1.5f;
    }

    if (m_iRenderState == Blok::BRS_NO_ATTACH)
    {
        fAlpha = .5f;
    }

    COLORREF darker =
        RGB(GetRValue(m_wColor) * fDarkerMod, GetGValue(m_wColor) * fDarkerMod,
            GetBValue(m_wColor) * fDarkerMod);

    COLORREF brighter = m_wColor;
    float newR = GetRValue(m_wColor) * fBrighterMod;
    float newG = GetGValue(m_wColor) * fBrighterMod;
    float newB = GetBValue(m_wColor) * fBrighterMod;

    if (newR > 255)
    {
        newR = 255;
    }

    if (newG > 255)
    {
        newG = 255;
    }

    if (newB > 255)
    {
        newB = 255;
    }

    brighter = RGB(newR, newG, newB);

    COLORREF brightest = m_wColor;
    newR = GetRValue(m_wColor) * 2.25f;
    newG = GetGValue(m_wColor) * 2.25f;
    newB = GetBValue(m_wColor) * 2.25f;

    if (newR > 255)
    {
        newR = 255;
    }

    if (newG > 255)
    {
        newG = 255;
    }

    if (newB > 255)
    {
        newB = 255;
    }

    brightest = RGB(newR, newG, newB);

    COLORREF normal = m_wColor;
    newR = GetRValue(m_wColor) * fMyMod;
    newG = GetGValue(m_wColor) * fMyMod;
    newB = GetBValue(m_wColor) * fMyMod;

    if (newR >= 255)
    {
        newR = 255;
    }

    if (newG >= 255)
    {
        newG = 255;
    }

    if (newB >= 255)
    {
        newB = 255;
    }

    normal = RGB(newR, newG, newB);

    unsigned int depth =
        ComputeDepth(); // RENDER.GetWidth() * ((RENDER.GetHeight() -
                        // m_iRenderY) + RENDER.GetWidth() - m_iRenderX);
    RENDER.Rectangle(m_iRenderX, m_iRenderY, m_iRenderX + m_width,
                     m_iRenderY + m_height, normal, depth, fAlpha);

    for (int i = 0; i < m_width / 2; i++)
    {
        if (i + 1 >= m_width / 2)
        {
            RENDER.VerticalLine(m_iRenderY - i, m_iRenderY + m_height - i,
                                m_iRenderX - i, 0, depth, fAlpha);
            RENDER.HorizontalLine(m_iRenderX - i, m_iRenderX + m_width - i,
                                  m_iRenderY - i, 0, depth, fAlpha);
        }
        else
        {
            RENDER.VerticalLine(m_iRenderY - i, m_iRenderY + m_height - i,
                                m_iRenderX - i, darker, depth, fAlpha);
            RENDER.HorizontalLine(m_iRenderX - i, m_iRenderX + m_width - i,
                                  m_iRenderY - i, brighter, depth, fAlpha);
        }

        RENDER.SetPixel(m_iRenderX - i, m_iRenderY + m_height - i, 0, depth,
                        fAlpha);

        if (m_poRight == NULL)
        {
            RENDER.SetPixel(m_iRenderX + m_width - i, m_iRenderY - i, 0,
                            depth, fAlpha);
        }
    }

    RENDER.HorizontalLine(m_iRenderX, m_iRenderX + m_width,
                          m_iRenderY + m_height, 0, depth, fAlpha);

    if (m_poRight == NULL)
    {
        RENDER.VerticalLine(m_iRenderY, m_iRenderY + m_height,
                            m_iRenderX + m_width, 0, depth, fAlpha);
    }

    if (m_poTop == NULL && m_iRenderState != BRS_NO_ATTACH)
    {
        // Draw Dot ////////////////////////////////////////////////////////
        //  Draw the dot thing that connects to other Bloks              //

        // RENDER.Circle((m_iRenderX + LEGO_WIDTH * .5f)-2, m_iRenderY -
        // LEGO_WIDTH/4.0f, LEGO_WIDTH
        // * .3f, darker, depth);

        int start = (int)(m_iRenderX + m_width * .15f);
        int end = (int)(m_iRenderX + m_width * .85f);
        int mid = (int)(m_iRenderX + m_width * .5f);

        int height = (int)(m_height * .25f);
        int radius = (int)(m_width * .35f);

        for (int i = start; i <= end; i++)
        {
            int baseY = m_iRenderY - m_width / 4;
            int baseX = i - m_width / 4;

            float realHeight = (.5f * sqrt((float)(radius * radius) -
                                           (float)((i - mid) * (i - mid))));
            float chopped = realHeight - ((int)realHeight);
            baseY += (int)(realHeight);

            float PercentDone = (float)(i - start) / (float)(end - start);
            COLORREF myColor;

            if (i == start || i == end)
            {
                myColor = 0;
            }
            else if (PercentDone <= .5f)
            {
                PercentDone *= 2.0f;
                float newR = GetRValue(darker) * (1.0f - PercentDone) +
                             GetRValue(normal) * PercentDone;
                float newG = GetGValue(darker) * (1.0f - PercentDone) +
                             GetGValue(normal) * PercentDone;
                float newB = GetBValue(darker) * (1.0f - PercentDone) +
                             GetBValue(normal) * PercentDone;

                myColor = RGB(newR, newG, newB);
            }
            else if (PercentDone <= .75f)
            {
                PercentDone -= .5f;
                PercentDone *= 4.0f;
                float newR = GetRValue(normal) * (1.0f - PercentDone) +
                             GetRValue(brightest) * PercentDone;
                float newG = GetGValue(normal) * (1.0f - PercentDone) +
                             GetGValue(brightest) * PercentDone;
                float newB = GetGValue(normal) * (1.0f - PercentDone) +
                             GetBValue(brightest) * PercentDone;

                myColor = RGB(newR, newG, newB);
            }
            else
            {
                PercentDone -= .75f;
                PercentDone *= 4.0f;
                float newR = GetRValue(brightest) * (1.0f - PercentDone) +
                             GetRValue(normal) * PercentDone;
                float newG = GetGValue(brightest) * (1.0f - PercentDone) +
                             GetGValue(normal) * PercentDone;
                float newB = GetBValue(brightest) * (1.0f - PercentDone) +
                             GetBValue(normal) * PercentDone;

                myColor = RGB(newR, newG, newB);
            }

            RENDER.VerticalLine(baseY - height, baseY, baseX, myColor, depth);

            if (i != start && i != end)
            {
                RENDER.VerticalLine(
                    baseY - 2 * (baseY - (m_iRenderY - m_width / 4)) -
                        height,
                    baseY - height, baseX, brighter, depth);
            }

            float AA = 1.0f - abs(chopped);
            float newR =
                GetRValue(brighter) * (1.0f - AA) + GetRValue(myColor) * AA;
            float newG =
                GetGValue(brighter) * (1.0f - AA) + GetGValue(myColor) * AA;
            float newB =
                GetBValue(brighter) * (1.0f - AA) + GetBValue(myColor) * AA;
            COLORREF AACol = RGB(newR, newG, newB);

            if (i != start && i != end)
            {
                RENDER.SetPixel(baseX, baseY - height, AACol, depth, fAlpha);

                RENDER.SetPixel(
                    baseX,
                    baseY - 2 * (baseY - (m_iRenderY - m_width / 4)) -
                        height,
                    RGB(newR * (1.0f - AA), newG * (1.0f - AA),
                        newB * (1.0f - AA)),
                    depth, fAlpha);
                RENDER.SetPixel(
                    baseX,
                    baseY - 2 * (baseY - (m_iRenderY - m_width / 4)) -
                        height - 1,
                    0, depth, (1.0f - AA) * fAlpha);
            }
            newR = GetRValue(myColor) * (1.0f - AA) + GetRValue(brighter) * AA;
            newG = GetGValue(myColor) * (1.0f - AA) + GetGValue(brighter) * AA;
            newB = GetBValue(myColor) * (1.0f - AA) + GetBValue(brighter) * AA;
            AACol = RGB(newR, newG, newB);
            if (i != start && i != end)
            {
                RENDER.SetPixel(baseX, baseY, AACol, depth, fAlpha);
            }
        }
    }
}

void Blok::ClearBlok(HDC hdc)
{
    HBRUSH NewBrush = CreateSolidBrush(CLEAR_COLOR);
    HPEN NewPen = CreatePen(PS_SOLID, 0, CLEAR_COLOR);

    SelectObject(hdc, NewBrush);
    SelectObject(hdc, NewPen);

    Rectangle(hdc, m_iRenderX, m_iRenderY, m_iRenderX + BLOK_WIDTH,
              m_iRenderY + BLOK_HEIGHT);

    DeleteObject(NewBrush);
    DeleteObject(NewPen);
}

bool Blok::CursorIntersectsPoint(int x, int y)
{
    if (!HasLeft())
    {
        if (x <= m_iX - BLOK_WIDTH / 2)
        {
            return false;
        }

        if (x >= m_iX + BLOK_WIDTH)
        {
            return false;
        }
    }
    else
    {
        if (x <= m_iX)
        {
            return false;
        }

        if (x >= m_iX + BLOK_WIDTH)
        {
            return false;
        }
    }

    if (!HasTop())
    {
        if (y <= m_iY - BLOK_WIDTH / 2)
        {
            return false;
        }

        if (y >= m_iY + BLOK_HEIGHT)
        {
            return false;
        }
    }
    else
    {
        if (y <= m_iY)
        {
            return false;
        }

        if (y >= m_iY + BLOK_HEIGHT)
        {
            return false;
        }
    }

    return true;
}

bool Blok::IntersectsPoint(int x, int y)
{
    if (x < m_iX)
    {
        return false;
    }
    if (x > m_iX + BLOK_WIDTH)
    {
        return false;
    }

    if (y > m_iY + BLOK_HEIGHT)
    {
        return false;
    }

    if (y < m_iY)
    {
        return false;
    }

    return true;
}

bool Blok::IntersectsBlok(Blok *poOther)
{
    if (std::abs(m_iX - poOther->m_iX) > BLOK_WIDTH)
        return false;
    if (std::abs(m_iY - poOther->m_iY) > BLOK_HEIGHT)
        return false;

    // We have an overlap
    return true;

    /*
    if (IntersectsPoint(poOther->m_iX, poOther->m_iY))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX + BLOK_WIDTH, poOther->m_iY))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX, poOther->m_iY + BLOK_HEIGHT))
    {
        return true;
    }

    if (IntersectsPoint(poOther->m_iX + BLOK_WIDTH, poOther->m_iY +
    BLOK_HEIGHT))
    {
        return true;
    }

    return false;
    */
}

bool Blok::IntersectsAnyBlok()
{
    SetMarks(1);

    bool ret = RecursiveIntersectsAnyBlok();
    SetMarks(0);

    return ret;
}

bool Blok::CanMateWithBlok(Blok *poOther)
{
    if (m_poBottom == NULL)
    {
        if (m_iY + BLOK_HEIGHT + (BLOK_MATE_LENGTH) > poOther->m_iY &&
            m_iY + BLOK_HEIGHT < poOther->m_iY &&
            m_iX > poOther->m_iX - BLOK_WIDTH / 2.0f &&
            m_iX < poOther->m_iX + BLOK_WIDTH / 2.0f)
        {
            GAMESTATE.SetMateUp(false);
            return true;
        }
    }

    if (m_poTop == NULL)
    {
        if (m_iY - BLOK_MATE_LENGTH < poOther->m_iY + BLOK_HEIGHT &&
            m_iY > poOther->m_iY + BLOK_HEIGHT &&
            m_iX > poOther->m_iX - BLOK_WIDTH / 2.0f &&
            m_iX < poOther->m_iX + BLOK_WIDTH / 2.0f)
        {
            GAMESTATE.SetMateUp(true);
            return true;
        }
    }

    return false;
}

Blok *Blok::FindMate()
{
    SetMarks(1);
    Blok *ret = RecursiveFindMate();
    SetMarks(1);

    if (ret == NULL)
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

    if (GAMESTATE.GetMovingMate()->m_iY < GAMESTATE.GetStaticMate()->m_iY)
    {
        GAMESTATE.GetMovingMate()->SetBlokRenderPosition(
            GAMESTATE.GetStaticMate()->m_iX,
            GAMESTATE.GetStaticMate()->m_iY -
                (int)(BLOK_HEIGHT + BLOK_MATE_LENGTH / 4.0f));
    }
    else
    {
        GAMESTATE.GetMovingMate()->SetBlokRenderPosition(
            GAMESTATE.GetStaticMate()->m_iX,
            GAMESTATE.GetStaticMate()->m_iY +
                (int)(BLOK_HEIGHT + BLOK_MATE_LENGTH / 4.0f));
    }
    return ret;
}

Blok *Blok::RecursiveFindMate()
{
    m_iMark = 2;

    Blok *b = GAMESTATE.MateSearch(this);

    if (b != NULL)
    {
        return b;
    }

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 1)
        {
            Blok *found = m_poLeft->RecursiveFindMate();

            if (found != NULL)
                return found;
        }
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 1)
        {
            Blok *found = m_poRight->RecursiveFindMate();

            if (found != NULL)
                return found;
        }
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 1)
        {
            Blok *found = m_poTop->RecursiveFindMate();

            if (found != NULL)
                return found;
        }
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 1)
        {
            Blok *found = m_poBottom->RecursiveFindMate();

            if (found != NULL)
                return found;
        }
    }

    return NULL;
}

void Blok::RecursiveMakeMatches()
{
    m_iMark = 2;

    m_poTopDesiredMate = NULL;
    m_poBottomDesiredMate = NULL;

    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        if (GAMESTATE.m_vpoBloks[i]->m_iMark == 3 &&
            CanMateWithBlok(GAMESTATE.m_vpoBloks[i]))
        {
            GAMESTATE.m_vpoBloks[i]->m_iMark = 4;
            if (GAMESTATE.m_vpoBloks[i]->m_iY < m_iY)
            {
                m_poTopDesiredMate = GAMESTATE.m_vpoBloks[i];
            }
            else
            {
                m_poBottomDesiredMate = GAMESTATE.m_vpoBloks[i];
            }
        }
    }

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 1)
        {
            m_poLeft->RecursiveMakeMatches();
        }
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 1)
        {
            m_poRight->RecursiveMakeMatches();
        }
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 1)
        {
            m_poTop->RecursiveMakeMatches();
        }
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 1)
        {
            m_poBottom->RecursiveMakeMatches();
        }
    }
}

void Blok::SetBlokPosition(int x, int y)
{
    RecursiveFixNeighbors(x, y);
    SetMarks(0);
}

void Blok::SetBlokRenderPosition(int x, int y)
{
    RecursiveFixRenderNeighbors(x, y);
    SetMarks(0);
}

void Blok::AttachLeft(Blok *poLeft, bool bMoveOther)
{
    m_poLeft = poLeft;
    if (bMoveOther)
        m_poLeft->SetBlokPosition(m_iX - BLOK_WIDTH, m_iY);
    if (bMoveOther)
        m_poLeft->SetBlokRenderPosition(m_iRenderX - BLOK_WIDTH, m_iRenderY);

    m_poLeft->m_poRight = this;
}

void Blok::AttachRight(Blok *poRight, bool bMoveOther)
{
    m_poRight = poRight;
    if (bMoveOther)
        m_poRight->SetBlokPosition(m_iX - BLOK_WIDTH, m_iY);
    if (bMoveOther)
        m_poRight->SetBlokRenderPosition(m_iRenderX - BLOK_WIDTH, m_iRenderY);

    m_poRight->m_poLeft = this;
}

void Blok::AttachTop(Blok *poTop, bool bMoveOther)
{
    m_poTop = poTop;
    if (bMoveOther)
        m_poTop->SetBlokPosition(m_iX, m_iY - BLOK_HEIGHT);

    m_poTop->m_poBottom = this;
}

void Blok::AttachBottom(Blok *poBottom, bool bMoveOther)
{
    m_poBottom = poBottom;

    if (bMoveOther)
        m_poBottom->SetBlokPosition(m_iX, m_iY + BLOK_HEIGHT);

    m_poBottom->m_poTop = this;
}

void Blok::SetMarks(int mark)
{
    if (m_iMark == mark)
    {
        return;
    }

    m_iMark = mark;

    if (m_poLeft != NULL && m_poLeft->m_iMark != mark)
    {
        m_poLeft->SetMarks(mark);
    }

    if (m_poRight != NULL && m_poRight->m_iMark != mark)
    {
        m_poRight->SetMarks(mark);
    }

    if (m_poTop != NULL && m_poTop->m_iMark != mark)
    {
        m_poTop->SetMarks(mark);
    }

    if (m_poBottom != NULL && m_poBottom->m_iMark != mark)
    {
        m_poBottom->SetMarks(mark);
    }
}

int Blok::ComputeDepth()
{
    if (HasLeft())
    {
        return m_poLeft->ComputeDepth();
    }
    /*else if (HasBottom())
    {
        return m_poBottom->ComputeDepth();
    }*/

    return ((RENDER.GetHeight() - m_iRenderY) * RENDER.GetWidth() +
            RENDER.GetWidth() - m_iRenderX);
}

void Blok::RecursiveSetGroup(int group)
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

void Blok::SetColorForMenu(int menu)
{
    int id = GetColorID();
    if (id == 1)
    {
        if (menu == MENU_AMETHYST)
        {
            m_wColor = COLOR_AMETHYST_0;
        }
        else if (menu == MENU_TOPAZ)
        {
            m_wColor = COLOR_TOPAZ_0;
        }
        else if (menu == MENU_PERIDOT)
        {
            m_wColor = COLOR_PERIDOT_0;
        }
        else
        {
            m_wColor = COLOR_RED;
        }
    }
    else if (id == 2)
    {
        if (menu == MENU_AMETHYST)
        {
            m_wColor = COLOR_AMETHYST_1;
        }
        else if (menu == MENU_TOPAZ)
        {
            m_wColor = COLOR_TOPAZ_1;
        }
        else if (menu == MENU_PERIDOT)
        {
            m_wColor = COLOR_PERIDOT_1;
        }
        else
        {
            m_wColor = COLOR_YELLOW;
        }
    }
    else if (id == 3)
    {
        if (menu == MENU_AMETHYST)
        {
            m_wColor = COLOR_AMETHYST_2;
        }
        else if (menu == MENU_TOPAZ)
        {
            m_wColor = COLOR_TOPAZ_2;
        }
        else if (menu == MENU_PERIDOT)
        {
            m_wColor = COLOR_PERIDOT_2;
        }
        else
        {
            m_wColor = COLOR_GREEN;
        }
    }
    else if (id == 4)
    {
        if (menu == MENU_AMETHYST)
        {
            m_wColor = COLOR_AMETHYST_3;
        }
        else if (menu == MENU_TOPAZ)
        {
            m_wColor = COLOR_TOPAZ_3;
        }
        else if (menu == MENU_PERIDOT)
        {
            m_wColor = COLOR_PERIDOT_3;
        }
        else
        {
            m_wColor = COLOR_BLUE;
        }
    }
}

void Blok::RecursiveFixNeighbors(int x, int y)
{
    if (m_iMark != 0)
    {
        return;
    }

    m_iMark = 1;

    m_iX = x;
    m_iY = y;

    if (m_physicsBodyId.index1 != 0)
    {
        Renderer::Pixel p(m_iX, m_iY);

        b2Vec2 phys = p.ToPhysics();

        b2Body_SetTransform(m_physicsBodyId, phys - m_shapePos, b2Rot_identity);
        b2Body_SetLinearVelocity(m_physicsBodyId, b2Vec2_zero);
    }

    if (m_poLeft != NULL)
    {
        m_poLeft->RecursiveFixNeighbors(x - BLOK_WIDTH, y);
    }

    if (m_poRight != NULL)
    {
        m_poRight->RecursiveFixNeighbors(x + BLOK_WIDTH, y);
    }

    if (m_poTop != NULL)
    {
        m_poTop->RecursiveFixNeighbors(x, y - BLOK_HEIGHT);
    }

    if (m_poBottom != NULL)
    {
        m_poBottom->RecursiveFixNeighbors(x, y + BLOK_HEIGHT);
    }
}

void Blok::RecursiveFixRenderNeighbors(int x, int y)
{
    if (m_iMark != 0)
    {
        return;
    }

    m_iMark = 1;

    m_iRenderX = x;
    m_iRenderY = y;

    if (m_poLeft != NULL)
    {
        m_poLeft->RecursiveFixRenderNeighbors(x - BLOK_WIDTH, y);
    }

    if (m_poRight != NULL)
    {
        m_poRight->RecursiveFixRenderNeighbors(x + BLOK_WIDTH, y);
    }

    if (m_poTop != NULL)
    {
        m_poTop->RecursiveFixRenderNeighbors(x, y - BLOK_HEIGHT);
    }

    if (m_poBottom != NULL)
    {
        m_poBottom->RecursiveFixRenderNeighbors(x, y + BLOK_HEIGHT);
    }
}

bool Blok::RecursiveIntersectsAnyBlok()
{
    m_iMark = 2;

    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        if (GAMESTATE.m_vpoBloks[i]->m_iMark == 0 &&
            GAMESTATE.m_vpoBloks[i]->IntersectsBlok(this))
        {
            return true;
        }
    }

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 1 && m_poLeft->RecursiveIntersectsAnyBlok())
            return true;
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 1 && m_poRight->RecursiveIntersectsAnyBlok())
            return true;
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 1 && m_poTop->RecursiveIntersectsAnyBlok())
            return true;
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 1 &&
            m_poBottom->RecursiveIntersectsAnyBlok())
            return true;
    }

    return false;
}

void Blok::RecursiveMate()
{
    m_iMark = 1;

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 0)
            m_poLeft->RecursiveMate();
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 0)
            m_poRight->RecursiveMate();
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 0)
            m_poTop->RecursiveMate();
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 0)
            m_poBottom->RecursiveMate();
    }

    if (m_poTopDesiredMate)
    {
        m_poTop = m_poTopDesiredMate;
        m_poTop->m_poBottom = this;
        m_poTopDesiredMate = NULL;
    }

    if (m_poBottomDesiredMate)
    {
        m_poBottom = m_poBottomDesiredMate;
        m_poBottom->m_poTop = this;
        m_poBottomDesiredMate = NULL;
    }
}

void Blok::Mate()
{

    std::set<b2BodyId> toDestroy;
    ClearPhysics(&toDestroy);
    GAMESTATE.GetStaticMate()->ClearPhysics(&toDestroy);

    for (std::set<b2BodyId>::iterator it = toDestroy.begin();
         it != toDestroy.end(); ++it)
    {
        PHYSICS.DestroyBody(*it);
    }

    RecursiveClearDistances();
    SetMarks(0);

    RecursiveClearHoldWeights();
    SetMarks(0);

    RecursiveMate();
    SetMarks(0);

    GAMESTATE.GetStaticMate()->SetMarks(0);

    GAMESTATE.GetStaticMate()->RecursiveFixRenderNeighbors(
        GAMESTATE.GetStaticMate()->m_iX, GAMESTATE.GetStaticMate()->m_iY);
    SetMarks(0);

    GAMESTATE.GetStaticMate()->RecursiveFixNeighbors(
        GAMESTATE.GetStaticMate()->m_iX, GAMESTATE.GetStaticMate()->m_iY);
    SetMarks(0);

    ClearMates();
    SetMarks(0);

    GAMESTATE.PlayAttach();
}

void Blok::ClearMates()
{
    m_iMark = 1;

    m_poBottomDesiredMate = m_poTopDesiredMate = NULL;

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 0)
            m_poLeft->ClearMates();
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 0)
            m_poRight->ClearMates();
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 0)
            m_poTop->ClearMates();
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 0)
            m_poBottom->ClearMates();
    }
}

bool Blok::CanDetachUp()
{
    m_iMark = 1;
    bool bIsAttached = false;

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 1;
    }

    for (Blok *iter = m_poRight; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 1;
    }

    int iNewMark = 2;
    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poBottom)
        {
            bIsAttached = true;
            iNewMark++;
            if (!iter->m_poBottom->RecursiveCanDetachBlokUp(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poBottom)
        {
            bIsAttached = true;
            iNewMark++;
            if (!iter->m_poBottom->RecursiveCanDetachBlokUp(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }

    SetMarks(0);
    return bIsAttached;
}

bool Blok::RecursiveCanDetachBlokUp(int mark)
{
    m_iMark = mark;

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 1)
            return false;

        if (m_poBottom->m_iMark != mark)
        {
            if (!m_poBottom->RecursiveCanDetachBlokUp(mark))
            {
                return false;
            }
        }
    }

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 1)
            return true;

        if (m_poLeft->m_iMark != mark)
        {
            if (!m_poLeft->RecursiveCanDetachBlokUp(mark))
            {
                return false;
            }
        }
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 1)
            return true;

        if (m_poRight->m_iMark != mark)
        {
            if (!m_poRight->RecursiveCanDetachBlokUp(mark))
            {
                return false;
            }
        }
    }

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 1)
            return true;

        if (m_poTop->m_iMark != mark)
        {
            if (!m_poTop->RecursiveCanDetachBlokUp(mark))
            {
                return false;
            }
        }
    }

    return true;
}

void Blok::DetachUp()
{
    std::set<b2BodyId> toDestroy;
    ClearPhysics(&toDestroy);

    for (std::set<b2BodyId>::iterator it = toDestroy.begin();
         it != toDestroy.end(); ++it)
    {
        PHYSICS.DestroyBody(*it);
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poBottom)
        {
            iter->m_poBottom->m_poTop = NULL;
            iter->m_poBottom = NULL;
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poBottom)
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

bool Blok::CanDetachDown()
{
    m_iMark = 1;
    bool bIsAttached = false;

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 1;
    }

    for (Blok *iter = m_poRight; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 1;
    }

    int iNewMark = 2;
    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poTop)
        {
            bIsAttached = true;
            iNewMark++;
            if (!iter->m_poTop->RecursiveCanDetachBlokDown(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poTop)
        {
            bIsAttached = true;
            iNewMark++;
            if (!iter->m_poTop->RecursiveCanDetachBlokDown(iNewMark))
            {
                SetMarks(0);
                return false;
            }
        }
    }

    SetMarks(0);
    return bIsAttached;
}

bool Blok::RecursiveCanDetachBlokDown(int mark)
{
    m_iMark = mark;

    if (m_poTop != NULL)
    {
        if (m_poTop->m_iMark == 1)
            return false;

        if (m_poTop->m_iMark != mark)
        {
            if (!m_poTop->RecursiveCanDetachBlokDown(mark))
            {
                return false;
            }
        }
    }

    if (m_poLeft != NULL)
    {
        if (m_poLeft->m_iMark == 1)
            return true;

        if (m_poLeft->m_iMark != mark)
        {
            if (!m_poLeft->RecursiveCanDetachBlokDown(mark))
            {
                return false;
            }
        }
    }

    if (m_poRight != NULL)
    {
        if (m_poRight->m_iMark == 1)
            return true;

        if (m_poRight->m_iMark != mark)
        {
            if (!m_poRight->RecursiveCanDetachBlokDown(mark))
            {
                return false;
            }
        }
    }

    if (m_poBottom != NULL)
    {
        if (m_poBottom->m_iMark == 1)
            return true;

        if (m_poBottom->m_iMark != mark)
        {
            if (!m_poBottom->RecursiveCanDetachBlokDown(mark))
            {
                return false;
            }
        }
    }

    return true;
}

void Blok::DetachDown()
{
    std::set<b2BodyId> toDestroy;
    ClearPhysics(&toDestroy);

    for (std::set<b2BodyId>::iterator it = toDestroy.begin();
         it != toDestroy.end(); ++it)
    {
        PHYSICS.DestroyBody(*it);
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poTop)
        {
            iter->m_poTop->m_poBottom = NULL;
            iter->m_poTop = NULL;
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poTop)
        {
            iter->m_poTop->m_poBottom = NULL;
            iter->m_poTop = NULL;
        }
    }

    GAMESTATE.PlayDetach();
}

void Blok::CalculateDistances()
{
    return;

    RecursiveClearDistances();
    SetMarks(0);
    RecursiveCalculateDistances(0);
    SetMarks(0);
}

void Blok::RecursiveClearDistances()
{
    if (m_iMark == 1)
    {
        return;
    }

    m_iMark = 1;
    m_iDistance = -1;

    if (m_poLeft != NULL)
    {
        m_poLeft->RecursiveClearDistances();
    }

    if (m_poRight != NULL)
    {
        m_poRight->RecursiveClearDistances();
    }

    if (m_poTop != NULL)
    {
        m_poTop->RecursiveClearDistances();
    }

    if (m_poBottom != NULL)
    {
        m_poBottom->RecursiveClearDistances();
    }
}

void Blok::RecursiveCalculateDistances(int distance)
{
    if (m_iMark == 1)
    {
        return;
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 1;
        iter->m_iDistance = distance;
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 1;
        iter->m_iDistance = distance;
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poTop)
        {
            iter->m_poTop->RecursiveCalculateDistances(distance + 1);
        }

        if (iter->m_poBottom)
        {
            iter->m_poBottom->RecursiveCalculateDistances(distance + 1);
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poTop)
        {
            iter->m_poTop->RecursiveCalculateDistances(distance + 1);
        }

        if (iter->m_poBottom)
        {
            iter->m_poBottom->RecursiveCalculateDistances(distance + 1);
        }
    }
}

void Blok::CalculateWeights()
{
    RecursiveClearHoldWeights();
    SetMarks(0);

    CalculateDistances();

    float w = GetWeight();

    char buff[300];
    sprintf_s(buff, 300, "Total Weight: %f", w);
}

float Blok::GetWeight()
{
    if (m_fBlockWeight >= 0)
    {
        return m_fBlockWeight;
    }

    float fMyWeight = 0;

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_iDistance != m_iDistance)
        {
            MessageBoxW(NULL, L"WRROR", L"Error", MB_OK);
        }

        if (iter->m_fTopWeightHeld < 0)
        {
            if (iter->m_poTop && iter->m_poTop->m_iDistance > m_iDistance)
            {
                iter->m_fTopWeightHeld = iter->m_poTop->GetWeight() /
                                         iter->m_poTop->GetNumConnections(iter);
            }
            else
            {
                iter->m_fTopWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fTopWeightHeld;

        if (iter->m_fBottomWeightHeld < 0)
        {
            if (iter->m_poBottom && iter->m_poBottom->m_iDistance > m_iDistance)
            {
                iter->m_fBottomWeightHeld =
                    iter->m_poBottom->GetWeight() /
                    iter->m_poBottom->GetNumConnections(iter);
            }
            else
            {
                iter->m_fBottomWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fBottomWeightHeld;

        fMyWeight += 1.0f;
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_fTopWeightHeld < 0)
        {
            if (iter->m_poTop && iter->m_poTop->m_iDistance > m_iDistance)
            {
                iter->m_fTopWeightHeld = iter->m_poTop->GetWeight() /
                                         iter->m_poTop->GetNumConnections(iter);
            }
            else
            {
                iter->m_fTopWeightHeld = 0;
            }
        }

        fMyWeight += iter->m_fTopWeightHeld;

        if (iter->m_fBottomWeightHeld < 0)
        {
            if (iter->m_poBottom && iter->m_poBottom->m_iDistance > m_iDistance)
            {
                iter->m_fBottomWeightHeld =
                    iter->m_poBottom->GetWeight() /
                    iter->m_poBottom->GetNumConnections(iter);
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
    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_fBlockWeight = fMyWeight;
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_fBlockWeight = fMyWeight;
    }

    return fMyWeight;
}

int Blok::GetNumConnections(Blok *poOther)
{
    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 1;
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 1;
    }

    int connections = 0;
    for (Blok *iter = poOther->m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poTop && iter->m_poTop->m_iMark == 1)
            connections++;

        if (iter->m_poBottom && iter->m_poBottom->m_iMark == 1)
            connections++;
    }

    for (Blok *iter = poOther; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poTop && iter->m_poTop->m_iMark == 1)
            connections++;

        if (iter->m_poBottom && iter->m_poBottom->m_iMark == 1)
            connections++;
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 0;
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 0;
    }

    return connections;
}

void Blok::RecursiveClearHoldWeights()
{
    if (m_iMark != 1)
    {
        m_iMark = 1;
        m_fBottomWeightHeld = m_fTopWeightHeld = -1.0f;

        m_fBlockWeight = -1.0f;

        if (m_poLeft != NULL)
        {
            m_poLeft->RecursiveClearHoldWeights();
        }

        if (m_poRight != NULL)
        {
            m_poRight->RecursiveClearHoldWeights();
        }

        if (m_poTop != NULL)
        {
            m_poTop->RecursiveClearHoldWeights();
        }

        if (m_poBottom != NULL)
        {
            m_poBottom->RecursiveClearHoldWeights();
        }
    }
}

void Blok::BreakStressedConnections()
{
    float f = (float)rand() / (float)RAND_MAX;
    f *= 1000;
    f += 8;

    RecursiveBreakStressedConnections(f);
    SetMarks(0);

    CalculateWeights();
}

void Blok::RecursiveBreakStressedConnections(float fThreshold)
{
    if (m_iMark == 1)
    {
        return;
    }

    m_iMark = 1;

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        iter->m_iMark = 1;
        if (iter->m_fBottomWeightHeld > fThreshold)
        {
            if (iter->m_poBottom &&
                iter->GetNumConnections(iter->m_poBottom) == 1)
            {
                iter->m_poBottom->m_poTop = NULL;
                iter->m_poBottom = NULL;
            }
        }

        if (iter->m_fTopWeightHeld > fThreshold)
        {
            if (iter->m_poTop && iter->GetNumConnections(iter->m_poTop) == 1)
            {
                iter->m_poTop->m_poBottom = NULL;
                iter->m_poTop = NULL;
            }
        }
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        iter->m_iMark = 1;
        if (iter->m_fBottomWeightHeld > fThreshold)
        {
            if (iter->m_poBottom &&
                iter->GetNumConnections(iter->m_poBottom) == 1)
            {
                iter->m_poBottom->m_poTop = NULL;
                iter->m_poBottom = NULL;
            }
        }

        if (iter->m_fTopWeightHeld > fThreshold)
        {
            if (iter->m_poTop && iter->GetNumConnections(iter->m_poTop) == 1)
            {
                iter->m_poTop->m_poBottom = NULL;
                iter->m_poTop = NULL;
            }
        }
    }

    for (Blok *iter = m_poLeft; iter != NULL; iter = iter->m_poLeft)
    {
        if (iter->m_poBottom)
            iter->m_poBottom->RecursiveBreakStressedConnections(fThreshold);

        if (iter->m_poTop)
            iter->m_poTop->RecursiveBreakStressedConnections(fThreshold);
    }

    for (Blok *iter = this; iter != NULL; iter = iter->m_poRight)
    {
        if (iter->m_poBottom)
            iter->m_poBottom->RecursiveBreakStressedConnections(fThreshold);

        if (iter->m_poTop)
            iter->m_poTop->RecursiveBreakStressedConnections(fThreshold);
    }
}

void Blok::SetRenderState(int iNewState)
{
    RecursiveSetRenderState(iNewState);
    SetMarks(0);
}

void Blok::RecursiveSetRenderState(int iNewState)
{
    if (m_iMark != 1)
    {
        m_iMark = 1;
        m_iRenderState = iNewState;

        if (m_poLeft != NULL)
        {
            m_poLeft->RecursiveSetRenderState(iNewState);
        }

        if (m_poRight != NULL)
        {
            m_poRight->RecursiveSetRenderState(iNewState);
        }

        if (m_poTop != NULL)
        {
            m_poTop->RecursiveSetRenderState(iNewState);
        }

        if (m_poBottom != NULL)
        {
            m_poBottom->RecursiveSetRenderState(iNewState);
        }
    }
}

bool Blok::PhysicsInited()
{
    return m_physicsBodyId.index1 != 0;
}

void Blok::InitPhysics(b2BodyId newBody, float offsetX, float offsetY)
{
    if (PhysicsInited())
    {
        return;
    }

    m_physicsBodyId = newBody;

    if (offsetX == 0 && offsetY == 0)
    {
        b2Vec2 center = GetPhysicsPos();
        offsetX = center.x;
        offsetY = center.y;
    }

    b2Vec2 pos = {offsetX, offsetY};

    m_shapePos = pos - b2Body_GetPosition(m_physicsBodyId);
    // m_b2v2FixturePos.x -= BLOK_WIDTH * .75f * PIXELS_TO_PHYSICS;
    // m_b2v2FixturePos.y -= BLOK_WIDTH * .75f * PIXELS_TO_PHYSICS;

    std::vector<b2Vec2> vertices;
    b2Vec2 point = {
        ((BLOK_WIDTH * 1.4f) * .5f + (BLOK_WIDTH * .2f)) * PIXELS_TO_PHYSICS,
        (BLOK_WIDTH * .25f + BLOK_HEIGHT * .5f) * PIXELS_TO_PHYSICS};
    vertices.push_back(m_shapePos + point);

    point = {((BLOK_WIDTH * 1.4f) * .5f + (BLOK_WIDTH * .2f)) *
                 PIXELS_TO_PHYSICS,
             -(BLOK_WIDTH * .25f + BLOK_HEIGHT * .5f) * PIXELS_TO_PHYSICS};
    vertices.push_back(m_shapePos + point);

    point = {(-(BLOK_WIDTH * 1.4f) * .5f + (BLOK_WIDTH * .2f)) *
                 PIXELS_TO_PHYSICS,
             -(BLOK_WIDTH * .25f + BLOK_HEIGHT * .5f) * PIXELS_TO_PHYSICS};
    vertices.push_back(m_shapePos + point);

    point = {(-(BLOK_WIDTH * 1.4f) * .5f + (BLOK_WIDTH * .2f)) *
                 PIXELS_TO_PHYSICS,
             (BLOK_WIDTH * .25f + BLOK_HEIGHT * .5f) * PIXELS_TO_PHYSICS};
    vertices.push_back(m_shapePos + point);

    b2Hull hull = b2ComputeHull(vertices.data(), vertices.size());

    b2Polygon shape =
        b2MakePolygon(&hull, 0); // (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;

    m_physicsShapeId = b2CreatePolygonShape(m_physicsBodyId, &shapeDef, &shape);

    if (m_poLeft != NULL)
    {
        m_poLeft->InitPhysics(
            newBody, offsetX - (float)BLOK_WIDTH * PIXELS_TO_PHYSICS, offsetY);
    }

    if (m_poRight != NULL)
    {
        m_poRight->InitPhysics(
            newBody, offsetX + (float)BLOK_WIDTH * PIXELS_TO_PHYSICS, offsetY);
    }

    if (m_poTop != NULL)
    {
        m_poTop->InitPhysics(newBody, offsetX,
                             offsetY + (float)BLOK_HEIGHT * PIXELS_TO_PHYSICS);
    }

    if (m_poBottom != NULL)
    {
        m_poBottom->InitPhysics(
            newBody, offsetX, offsetY - (float)BLOK_HEIGHT * PIXELS_TO_PHYSICS);
    }
}

void Blok::ClearPhysics(std::set<b2BodyId> *pBodies)
{
    if (m_physicsBodyId.index1 == 0)
    {
        return;
    }

    pBodies->insert(m_physicsBodyId);
    b2DestroyShape(m_physicsShapeId, false);

    m_physicsBodyId = b2BodyId();
    m_physicsShapeId = b2ShapeId();

    if (m_poLeft != NULL)
    {
        m_poLeft->ClearPhysics(pBodies);
    }

    if (m_poRight != NULL)
    {
        m_poRight->ClearPhysics(pBodies);
    }

    if (m_poTop != NULL)
    {
        m_poTop->ClearPhysics(pBodies);
    }

    if (m_poBottom != NULL)
    {
        m_poBottom->ClearPhysics(pBodies);
    }
}

void Blok::SetPosFromPhysics()
{
    if (PhysicsEnabled() && !IsMateEnabled())
    {
        Renderer::Pixel p(b2Body_GetPosition(m_physicsBodyId) + m_shapePos);
        m_iRenderX = m_iX = (int)(p.m_iX); // -BLOK_WIDTH * .25f;
        m_iRenderY = m_iY = (int)(p.m_iY - (float)BLOK_WIDTH * .25f);
    }
}

b2Vec2 Blok::GetPhysicsPos()
{
    Renderer::Pixel p(m_iRenderX, m_iRenderY);
    b2Vec2 pos = p.ToPhysics();

    return pos;
}

bool Blok::IsMateEnabled()
{
    if (PhysicsInited())
    {
        Blok *b = (Blok *)b2Body_GetUserData(m_physicsBodyId);
        if (b && b == this)
        {
            return m_bMateEnabled;
        }
        else if (b)
        {
            return b->IsMateEnabled();
        }
    }
    return false;
}

void Blok::MarkMateDisabled()
{
    if (PhysicsInited())
    {
        Blok *b = (Blok *)b2Body_GetUserData(m_physicsBodyId);
        if (b && b == this)
        {
            m_bMateEnabled = false;
        }
        else if (b)
        {
            b->MarkMateDisabled();
        }
    }
}

void Blok::MarkMateEnabled()
{
    if (PhysicsInited())
    {
        Blok *b = (Blok *)b2Body_GetUserData(m_physicsBodyId);
        if (b && b == this)
        {
            m_bMateEnabled = true;
        }
        else if (b)
        {
            b->MarkMateEnabled();
        }
    }
}

bool Blok::PhysicsEnabled()
{
    if (PhysicsInited())
    {
        return b2Body_IsEnabled(m_physicsBodyId);
    }
    return false;
}

void Blok::SetStatic(bool bStatic)
{
    if (PhysicsInited())
    {
        b2Body_SetAwake(m_physicsBodyId, true);
        b2Body_SetType(m_physicsBodyId,
                       bStatic ? b2_staticBody : b2_dynamicBody);
    }
}