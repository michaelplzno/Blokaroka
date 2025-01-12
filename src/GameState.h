/**
 * @file GameState.h
 * @brief This file contains the definition of the GameState enumeration.
 *
 * The game state is largley a list of the current game objects and their data.
 * It contains a list of Bloks (Blok.cpp) and manages their interactions.
 *
 * The GameState is a singleton that can be accessed with GAMESTATE. There
 * should be only one GameState in existence ever.
 */

#ifndef __GameState_H__
#define __GameState_H__

#include "Blokaroka.h"
#include <map>
#include <string>

// Size of Bloks in pixels.
#define BLOK_WIDTH 15
#define BLOK_HEIGHT 18

// Gap at which Bloks can find another block to mate with.
#define BLOK_MATE_LENGTH 15

// Basic Primary Color Scheme.
#define COLOR_WHITE RGB(242, 243, 242)
#define COLOR_RED RGB(196, 40, 27)
#define COLOR_YELLOW RGB(245, 205, 47)
#define COLOR_GREEN RGB(75, 151, 74)
#define COLOR_BLUE RGB(13, 105, 171)

// Amethyst Color Scheme.
#define COLOR_AMETHYST_0 RGB(254, 233, 243)
#define COLOR_AMETHYST_1 RGB(238, 166, 201)
#define COLOR_AMETHYST_2 RGB(214, 114, 151)
#define COLOR_AMETHYST_3 RGB(170, 90, 127)

// Topaz Color Scheme.
#define COLOR_TOPAZ_0 RGB(190, 229, 227)
#define COLOR_TOPAZ_1 RGB(118, 206, 215)
#define COLOR_TOPAZ_2 RGB(60, 186, 218)
#define COLOR_TOPAZ_3 RGB(45, 133, 189)

// Peridot Color Scheme.
#define COLOR_PERIDOT_0 RGB(199, 251, 155)
#define COLOR_PERIDOT_1 RGB(145, 217, 85)
#define COLOR_PERIDOT_2 RGB(102, 178, 49)
#define COLOR_PERIDOT_3 RGB(65, 131, 28)

// The GameState class is a singleton that manages the game state.
class GameState
{
  public:

    // Game state for what the global status of the game is.
    enum
    {
        GS_Static, // Bloks are not being interacted with, but due to phyics may still be moving.
        GS_Drag_Free, // Bloks are being dragged around freely.
        GS_Drag_Mating, // Bloks are being dragged for mating, and will snap to
                        // the nearest mate.
        GS_Drag_Splitting // When bloks have just been split, they are in this
                          // state.
    };

    /**
     * @brief Constructor for the GameState class.
     *
     * This constructor initializes the GameState object.
     */
    GameState()
        : m_iMateDeltaX(0), m_iMateDeltaY(0), m_iState(0), m_poMovingMate(NULL),
          m_poStaticMate(NULL), m_poSelectedBlok(NULL), m_vpoBloks(NULL)
    {
    }

    void GenerateRandomBloks(); // Generate a random gamestate at the beginning
                                // of a run or on reset.
    void Update();       // Call this every frame to update Bolk positions from
                         // physics engine.
    void Draw(HDC hdc);  // Draw the current gamestate to the screen.
    void ClearBloks();   // Delete the entire list of Bloks and destroy their
                         // physics entities.
    void RegenPhysics(); // Find any Blok without a physics entity and create
                         // it.

    void SetState(
        int iNewState); // Use one of the GS_ enums to set the game state.
    int GetState();     // Get the current game state.

    void SetDragging(Blok *poDragged, int x, int y);
    void MoveSelected(int x, int y, int deltaX, int deltaY);
    void SetSplitDragging(Blok *poDragged, int x, int y);

    Blok *GetBlokAt(int x, int y);

    void PlayAttach(); // Play a random attach sound.
    void PlayDetach(); // Play a random detach sound.

    void DumpGamestate(
        std::wstring cstrName); // Write the current gamestate to a file.
    bool ReadGamestate(std::wstring cstrName); // Read a gamestate from a file.

    void SetColorsForMenu(
        int menu); // Reset blok colors based on which menu ID is chosen.

    Blok *GetMovingMate()
    {
        return m_poMovingMate;
    }

    Blok *GetStaticMate()
    {
        return m_poStaticMate;
    }

    void SetMateUp(bool bUp)
    {
        m_bMateUp = bUp;
    }

    Blok *MateSearch(Blok *poBlok);

    std::vector<Blok *> m_vpoBloks; // List of all Bloks in the game.

  private:
    bool m_bMateUp; // True if the mate is above the current blok.

    Blok *m_poMovingMate; // Blok that is currently being dragged.
    Blok *m_poStaticMate; // Blok that is currently being mated with.
    Blok *m_poSelectedBlok; // Blok that is currently selected.

    int m_iState; // Current game state, usually GS_Static.

    int m_iMateDeltaX; // Amount the mouse has moved (X) since mating started.
    int m_iMateDeltaY; // Amount the mouse has moved (Y) since mating started.
};

extern GameState GAMESTATE;

#endif // __GameState_H__