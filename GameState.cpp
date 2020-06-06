#include "GameState.h"

GameState GAMESTATE;



void GameState::GenerateBloks()
{
    for( int i = 0; i < 100; i++ )
    {
        COLORREF color = RGB(0, 0, 0);
        int colorNum = rand() % 5;
        if(colorNum == 0)
        {
            color = COLOR_WHITE;
        } 
        else if(colorNum == 1)
        {
            color = COLOR_RED;
        } 
        else if(colorNum == 2)
        {
            color = COLOR_YELLOW;
        }
        else if(colorNum == 3)
        {
            color = COLOR_GREEN;
        }
        else if(colorNum == 4)
        {
            color = COLOR_BLUE;
        }

        int width = rand()%3;

        m_vpoBloks.push_back(new Blok(color, rand() % 1000, rand() % 768));
        
        for(int i = 0; i < pow(2.0f,width)- 1; i++)
        {
            Blok* newLego = new Blok(color, rand() % 1000, rand() % 768);
            m_vpoBloks.back()->AttachLeft(newLego);
            m_vpoBloks.push_back(newLego);
        }
    }
}

void GameState::Shutdown()
{

}

void GameState::Update()
{
    for (unsigned int i = 0; i < m_vpoBloks.size(); i++)
    {
        if (!m_vpoBloks[i]->PhysicsInited())
        {
            m_vpoBloks[i]->SetBlokRenderPosition(m_vpoBloks[i]->GetX(), m_vpoBloks[i]->GetY());

            b2Body* body = PHYSICS.GenerateBody();
            body->SetTransform(m_vpoBloks[i]->GetPhysicsPos(), 0);
            body->SetUserData(m_vpoBloks[i]);

            m_vpoBloks[i]->InitPhysics(body);
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
        std::set<b2Body*> toDestroy;
        m_vpoBloks[i]->ClearPhysics(&toDestroy);

        for (std::set<b2Body*>::iterator it = toDestroy.begin(); it != toDestroy.end(); ++it)
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
            m_vpoBloks[i]->SetBlokRenderPosition(m_vpoBloks[i]->GetX(), m_vpoBloks[i]->GetY());

            b2Body* body = PHYSICS.GenerateBody();
            body->SetTransform(m_vpoBloks[i]->GetPhysicsPos(), 0);
            body->SetUserData(m_vpoBloks[i]);

            m_vpoBloks[i]->InitPhysics(body);
        }
    }
}


void GameState::SetState(int iNewState)
{
    if (m_poSelectedBlok != NULL)
    {
        m_poSelectedBlok->EnablePhysics();
    }

    if (iNewState == GS_Static)
    {
        if (m_iState == GS_Drag_Mating)
        {
            PHYSICS.OnMouseUp();

            m_poSelectedBlok->Mate();

            std::set<b2Body*> toDestroy;
            m_poSelectedBlok->ClearPhysics(&toDestroy);

            for (std::set<b2Body*>::iterator it = toDestroy.begin(); it != toDestroy.end(); ++it)
            {
                PHYSICS.DestroyBody(*it);
            }

            m_poSelectedBlok->SetMarks(0);
        }
        m_poSelectedBlok->SetRenderState(Blok::BRS_SOLID);
    }
    else if( iNewState == GS_Drag_Free )
    {

    }
    else if (iNewState == GS_Drag_Mating)
    {
        if (m_poSelectedBlok != NULL)
        {
            m_poSelectedBlok->DisablePhysics();
        }
    }

        
    m_iState = iNewState;
}

int GameState::GetState()
{
    return m_iState;
}

void GameState::SetDragging(Blok* poDragged, int x, int y)
{
    m_poSelectedBlok = poDragged;
    m_iXOffset = m_poSelectedBlok->GetX() - x;
    m_iYOffset = m_poSelectedBlok->GetY() - y;
    SetState(GS_Drag_Free);
    m_poSelectedBlok->SetRenderState(Blok::BRS_GLOW);
    //m_poSelectedLego->CalculateWeights();
}

void GameState::SetSplitDragging(Blok* poDragged, int x, int y)
{
    m_poSelectedBlok = poDragged;
    m_iXOffset = m_poSelectedBlok->GetX() - x;
    m_iYOffset = m_poSelectedBlok->GetY() - y;
    SetState(GS_Drag_Splitting);
    m_poSelectedBlok->SetRenderState(Blok::BRS_NO_ATTACH);
    //m_poSelectedLego->CalculateWeights();
}

void  GameState::MoveSelected(int x, int y)
{

    if (m_poSelectedBlok != NULL && !m_poSelectedBlok->PhysicsEnabled())
    {
        m_poSelectedBlok->SetBlokPosition(m_iXOffset + x, m_iYOffset + y);
    }

    //m_poSelectedLego->BreakStressedConnections();

    float delta = sqrt((float)((x - m_iLastX) * (x - m_iLastX) + 
                               (y - m_iLastY) * (y - m_iLastY)));

    m_iLastX = x;
    m_iLastY = y;
    
    /*
    if(false)
    {
        if(m_iState == GS_Drag_Mating && m_poSelectedBlok->IntersectsAnyBlok())
        {
            m_poSelectedBlok->Mate();
            m_iState = GS_Static;
        }
        else
        {
            SetState(GS_Drag_Free);
            m_poSelectedBlok->SetBlokRenderPosition(m_iXOffset + x, m_iYOffset +  y);
        }
        return;
    }
    */

    if(m_iState != GS_Drag_Splitting)
    {
        if(m_poSelectedBlok->IntersectsAnyBlok())
        {
            if(m_iState == GS_Drag_Mating)
            {
                PHYSICS.OnMouseUp();

                m_poSelectedBlok->Mate();

                std::set<b2Body*> toDestroy;
                m_poSelectedBlok->ClearPhysics(&toDestroy);

                for (std::set<b2Body*>::iterator it = toDestroy.begin(); it != toDestroy.end(); ++it)
                {
                    PHYSICS.DestroyBody(*it);
                }

                m_poSelectedBlok->SetRenderState(Blok::BRS_SOLID);
                m_iState = GS_Static;
            }
            else
            {
                //m_poSelectedBlok->SetBlokRenderPosition(m_iXOffset + x, m_iYOffset +  y);
            }
        }
        else
        {
            Blok *mate = m_poSelectedBlok->FindMate();
            if(mate)
            {
                SetState(GS_Drag_Mating);
            }
            else
            {
                SetState(GS_Drag_Free);
                //m_poSelectedBlok->SetBlokRenderPosition(m_iXOffset + x, m_iYOffset +  y);
            }
        }
    }
    else
    {
        //m_poSelectedBlok->SetBlokRenderPosition(m_iXOffset + x, m_iYOffset +  y);
    }
}

Blok* GameState::GetBlokAt(int x, int y)
{
    for( unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++ )
    {
        if (GAMESTATE.m_vpoBloks[i]->CursorIntersectsPoint(x,y))
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

    out.open(wstrName.c_str(), std::ofstream::out );


    std::map<const Blok*, int> indexMap;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++ )
    {
        indexMap[m_vpoBloks[i]] = i;
        out << i << " " 
            << m_vpoBloks[i]->GetX() << " " 
            << m_vpoBloks[i]->GetY() << " "
            << m_vpoBloks[i]->GetColor() << " "
            << std::endl;
    }

    out << "-1" << std::endl;

    for( unsigned int i = 0; i < GAMESTATE.m_vpoBloks.size(); i++ )
    {
        out << i << " ";

        if(m_vpoBloks[i]->HasLeft())
        {
            out << indexMap[m_vpoBloks[i]->GetLeft()] << " ";
        }
        else
        {
            out << -1 << " ";
        }

        if(m_vpoBloks[i]->HasTop())
        {
            out << indexMap[m_vpoBloks[i]->GetTop()] << " ";
        }
        else
        {
            out << -1 << " ";
        }


        if(m_vpoBloks[i]->HasRight())
        {
            out << indexMap[m_vpoBloks[i]->GetRight()] << " ";
        }
        else
        {
            out << -1 << " ";
        }


        if(m_vpoBloks[i]->HasBottom())
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


bool GameState::ReadGamestate(std::wstring wstrName )
{

    std::ifstream in;

    in.open(wstrName, std::ofstream::in );

    if(!in.is_open())
    {
        return false;
    }

    for( unsigned int ui = 0; ui < GAMESTATE.m_vpoBloks.size(); ui++ )
    {
        delete m_vpoBloks[ui];
        m_vpoBloks[ui] = 0;
    }

    m_vpoBloks.clear();

    int i, x, y;
    COLORREF color;

    in >> i;
    while(i >= 0)
    {
        in >> x >> y >> color;
        Blok* temp = new Blok(color,x,y);

        m_vpoBloks.push_back(temp);

        in >> i;
    }

    

    int left, right, top, bottom;

    while(in >> i >> left >> top >> right >> bottom)
    {
        if(left >= 0)
        {
            m_vpoBloks[i]->AttachLeft(m_vpoBloks[left], false);
        }

        if(top >= 0)
        {
            m_vpoBloks[i]->AttachTop(m_vpoBloks[top], false);
        }

        if(bottom >= 0)
        {
            m_vpoBloks[i]->AttachBottom(m_vpoBloks[bottom], false);
        }

        if(right >= 0)
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