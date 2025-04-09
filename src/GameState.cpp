#define NOMINMAX
#include "GameState.h"

GameState GAMESTATE;

void GameState::GenerateRandomBloks()
{
    for (int i = 0; i < 100; i++)
    {
        COLORREF color = RGB(0, 0, 0);
        int colorNum = rand() % 5;
        if (colorNum == 0)
        {
            color = COLOR_WHITE;
        }
        else if (colorNum == 1)
        {
            color = COLOR_RED;
        }
        else if (colorNum == 2)
        {
            color = COLOR_YELLOW;
        }
        else if (colorNum == 3)
        {
            color = COLOR_GREEN;
        }
        else if (colorNum == 4)
        {
            color = COLOR_BLUE;
        }

        int width = rand() % 3;

        m_vpoBloks.push_back(new Blok(color, rand() % 1000, rand() % 768));

        for (int i = 0; i < pow(2.0f, width) - 1; i++)
        {
            Blok *newLego = new Blok(color, rand() % 1000, rand() % 768);
            m_vpoBloks.back()->AttachLeft(newLego);
            m_vpoBloks.push_back(newLego);
        }
    }
}

void GameState::Update()
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {

        if (!m_vpoBloks[i]->PhysicsInited())
        {
            b2BodyId body = PHYSICS.GenerateBody();
            b2Rot rot = b2Rot_identity;
            b2Body_SetTransform(body, m_vpoBloks[i]->GetPhysicsPos(), rot);
            b2Body_SetUserData(body, m_vpoBloks[i]);

            m_vpoBloks[i]->InitPhysics(body);

            m_vpoBloks[i]->SetBlokRenderPosition(m_vpoBloks[i]->GetX(),
                                                 m_vpoBloks[i]->GetY());
        }
        else
        {
            m_vpoBloks[i]->SetPosFromPhysics();
        }
    }
}

void GameState::ClearBloks()
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        std::set<b2BodyId> toDestroy;
        m_vpoBloks[i]->ClearPhysics(&toDestroy);

        for (std::set<b2BodyId>::iterator it = toDestroy.begin();
             it != toDestroy.end(); ++it)
        {
            PHYSICS.DestroyBody(*it);
        }

        delete m_vpoBloks[i];
    }

    m_vpoBloks.clear();
}

void GameState::RegenPhysics()
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        if (!m_vpoBloks[i]->PhysicsInited())
        {
            m_vpoBloks[i]->SetBlokRenderPosition(m_vpoBloks[i]->GetX(),
                                                 m_vpoBloks[i]->GetY());

            b2BodyId body = PHYSICS.GenerateBody();
            b2Rot rot = b2Rot_identity;
            b2Body_SetTransform(body, m_vpoBloks[i]->GetPhysicsPos(), rot);
            b2Body_SetUserData(body, m_vpoBloks[i]);

            m_vpoBloks[i]->InitPhysics(body);
        }
    }
}

void GameState::SetState(int iNewState)
{

    if (iNewState == m_iState)
    {
        return;
    }

    if (m_poSelectedBlok != NULL)
    {
        m_poSelectedBlok->MarkMateDisabled();
    }

    if (iNewState == GS_Static)
    {
        if (m_iState == GS_Drag_Mating)
        {
            PHYSICS.OnMouseUp();

            if (m_poSelectedBlok != NULL)
            {
                m_poSelectedBlok->Mate();

                std::set<b2BodyId> toDestroy;
                m_poSelectedBlok->ClearPhysics(&toDestroy);
                if (m_poStaticMate != NULL)
                {
                    m_poStaticMate->ClearPhysics(&toDestroy);
                }

                for (std::set<b2BodyId>::iterator it = toDestroy.begin();
                     it != toDestroy.end(); ++it)
                {
                    PHYSICS.DestroyBody(*it);
                }

                m_poSelectedBlok->SetMarks(0);
            }
        }

        if (m_poSelectedBlok != NULL)
        {
            m_poSelectedBlok->SetRenderState(Blok::BRS_SOLID);
        }
        m_poSelectedBlok = NULL;
        m_poStaticMate = NULL;
        m_iMateDeltaX = m_iMateDeltaY = 0;
    }
    else if (iNewState == GS_Drag_Free)
    {
        if (m_poStaticMate != NULL)
        {
            m_poStaticMate->SetStatic(false);
            m_poStaticMate->MarkMateDisabled();
            m_poStaticMate = NULL;
        }
    }
    else if (iNewState == GS_Drag_Mating)
    {
        if (m_poSelectedBlok != NULL)
        {
            m_poSelectedBlok->MarkMateEnabled();
        }
    }

    m_iState = iNewState;
}

int GameState::GetState()
{
    return m_iState;
}

void GameState::SetDragging(Blok *poDragged, int x, int y)
{
    m_poSelectedBlok = poDragged;
    SetState(GS_Drag_Free);
    m_poSelectedBlok->SetRenderState(Blok::BRS_GLOW);
    // m_poSelectedLego->CalculateWeights();
}

void GameState::SetSplitDragging(Blok *poDragged, int x, int y)
{
    m_poSelectedBlok = poDragged;
    SetState(GS_Drag_Splitting);
    m_poSelectedBlok->SetRenderState(Blok::BRS_NO_ATTACH);
    // m_poSelectedLego->CalculateWeights();
}

void GameState::MoveSelected(int x, int y, int deltaX, int deltaY)
{
    if (m_iState == GS_Drag_Mating)
    {
        m_iMateDeltaX += deltaX;
        m_iMateDeltaY += deltaY;

        if ((m_iMateDeltaY < -BLOK_MATE_LENGTH * .5f && m_bMateUp) ||
            (m_iMateDeltaY > BLOK_MATE_LENGTH * .5f && !m_bMateUp))

        {
            PHYSICS.OnMouseUp();

            SetState(GS_Static);
        }
        else if (std::abs(m_iMateDeltaX) > 1.5f * BLOK_MATE_LENGTH ||
                 std::abs(m_iMateDeltaY) > 0.6f * BLOK_MATE_LENGTH)
        {
            m_poStaticMate->SetStatic(false);
            m_poStaticMate->MarkMateDisabled();
            m_poSelectedBlok->MarkMateDisabled();

            SetState(GS_Drag_Free);
        }
    }
    else
    {
        m_iMateDeltaX = 0;
        m_iMateDeltaY = 0;

        if (m_iState != GS_Drag_Splitting && m_poSelectedBlok)
        {
            Blok *mate = m_poSelectedBlok->FindMate();
            if (mate)
            {
                SetState(GS_Drag_Mating);
            }
            else
            {
                SetState(GS_Drag_Free);
            }
        }
    }
}

Blok *GameState::GetBlokAt(int x, int y)
{
    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        if (GAMESTATE.m_vpoBloks[i]->CursorIntersectsPoint(x, y))
        {
            return GAMESTATE.m_vpoBloks[i];
        }
    }

    return NULL;
}

void GameState::PlayAttach()
{
    int effect = rand() % 4;
    if (effect == 0)
    {
        gPlaySound(IDR_ATTACH0);
    }
    if (effect == 1)
    {
        gPlaySound(IDR_ATTACH1);
    }
    if (effect == 2)
    {
        gPlaySound(IDR_ATTACH2);
    }
    if (effect == 3)
    {
        gPlaySound(IDR_ATTACH3);
    }
}

void GameState::PlayDetach()
{
    int effect = rand() % 4;
    if (effect == 0)
    {
        gPlaySound(IDR_DETACH0);
    }
    if (effect == 1)
    {
        gPlaySound(IDR_DETACH1);
    }
    if (effect == 2)
    {
        gPlaySound(IDR_DETACH2);
    }
    if (effect == 3)
    {
        gPlaySound(IDR_DETACH3);
    }
}

void GameState::DumpGamestate(std::wstring wstrName)
{
    CreateDirectoryW(gAppDataDirectory().c_str(), 0);

    std::ofstream out;

    out.open(wstrName.c_str(), std::ofstream::out);

    if (PHYSICS.IsGravityOn())
    {
        out << "~g 1" << std::endl;
    }
    else
    {
        out << "~g 0" << std::endl;
    }

    if (PHYSICS.IsDebugRenderOn())
    {
        out << "~d 1" << std::endl;
    }
    else
    {
        out << "~d 0" << std::endl;
    }

    std::map<const Blok *, int> indexMap;

    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        indexMap[m_vpoBloks[i]] = i;
        out << i << " " << m_vpoBloks[i]->GetX() << " " << m_vpoBloks[i]->GetY()
            << " " << m_vpoBloks[i]->GetColor() << " " << std::endl;
    }

    out << "-1" << std::endl;

    for (unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++)
    {
        out << i << " ";

        if (m_vpoBloks[i]->HasLeft())
        {
            out << indexMap[m_vpoBloks[i]->GetLeft()] << " ";
        }
        else
        {
            out << -1 << " ";
        }

        if (m_vpoBloks[i]->HasTop())
        {
            out << indexMap[m_vpoBloks[i]->GetTop()] << " ";
        }
        else
        {
            out << -1 << " ";
        }

        if (m_vpoBloks[i]->HasRight())
        {
            out << indexMap[m_vpoBloks[i]->GetRight()] << " ";
        }
        else
        {
            out << -1 << " ";
        }

        if (m_vpoBloks[i]->HasBottom())
        {
            out << indexMap[m_vpoBloks[i]->GetBottom()] << std::endl;
        }
        else
        {
            out << -1 << std::endl;
        }
    }

    out.close();
}

bool GameState::ReadGamestate(std::wstring wstrName)
{

    std::ifstream in;

    in.open(wstrName, std::ofstream::in);

    if (!in.is_open())
    {
        return false;
    }

    while (in.peek() == '~')
    {
        char tilda;
        char setting;
        int value;
        in >> tilda >> setting >> value;
        if (setting == 'g')
        {
            PHYSICS.ChangeGravity(value != 0 ? -10.0f : 0.0f);
        }
        else if (setting == 'd')
        {
            PHYSICS.SetDebugRender(value != 0);
        }
        else if (setting == 'c')
        {
            SetColorsForMenu(value);
        }

        // Clear trailing whitespace
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    for (unsigned int ui = 0; ui < GAMESTATE.m_vpoBloks.size(); ui++)
    {
        delete m_vpoBloks[ui];
        m_vpoBloks[ui] = 0;
    }

    m_vpoBloks.clear();

    int i, x, y;
    COLORREF color;

    in >> i;
    while (i >= 0)
    {
        in >> x >> y >> color;
        Blok *temp = new Blok(color, x, y);

        m_vpoBloks.push_back(temp);

        in >> i;
    }

    int left, right, top, bottom;

    while (in >> i >> left >> top >> right >> bottom)
    {
        if (left >= 0)
        {
            m_vpoBloks[i]->AttachLeft(m_vpoBloks[left], false);
        }

        if (top >= 0)
        {
            m_vpoBloks[i]->AttachTop(m_vpoBloks[top], false);
        }

        if (bottom >= 0)
        {
            m_vpoBloks[i]->AttachBottom(m_vpoBloks[bottom], false);
        }

        if (right >= 0)
        {
            m_vpoBloks[i]->AttachRight(m_vpoBloks[right], false);
        }
    }
    in.close();
    return true;
}

void GameState::SetColorsForMenu(int menu)
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        m_vpoBloks[i]->SetColorForMenu(menu);
    }

    RENDER.RenderFrame();
    RENDER.PresentFrame();
}

Blok *GameState::MateSearch(Blok *poBlok)
{

    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        if (m_vpoBloks[i]->GetMark() == 0 &&
            poBlok->CanMateWithBlok(m_vpoBloks[i]))
        {
            m_poMovingMate = poBlok;
            m_poStaticMate = m_vpoBloks[i];

            m_poStaticMate->SetStatic(true);

            return m_vpoBloks[i];
        }
    }
    return NULL;
}

void GameState::Draw(HDC hdc)
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        m_vpoBloks[i]->DrawBlok(hdc);
    }
}

void GameState::ResetPhysics()
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        m_vpoBloks[i]->SetStatic(true);
        m_vpoBloks[i]->SetStatic(false);
    }
}