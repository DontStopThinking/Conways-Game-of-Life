/*
Conway's game of life rules:
For living cell:
    - less than 2 living neighbors = die (underpopulation)
    - 2 or 3 living neighbors = live
    - more than 3 living neighbors = die (overpopulation)

For dead cell:
    - 3 living neighbors = come to life
*/
#include <iostream>
#include "raylib.h"

#include "game_of_life.h"
#include "colors.h"

void drawGame(void);
void updateGame(void);
void updateAndDrawGame(void);

// window needs to be square
constexpr int winWidth = 600;
constexpr int winHeight = 600;
int m_cellSize = 10;

GameOfLife simulation(winWidth, winHeight, m_cellSize);

int main()
{
    std::cout << "\tCONWAY'S GAME OF LIFE\n\n";
    std::cout << "\t  - by Shantanu Balse\n";
    std::cout << "\t    (github.com/shantanubalse)\n\n";

    std::cout << "Basic Controls:\n";
    std::cout << "  Space: run/pause the simulation\n";
    std::cout << "  Left click: place new cell when paused\n";
    std::cout << "  Right click: remove cell when paused\n";
    std::cout << "  C: remove all cells\n\n";

    std::cout << "Colors:\n";
    std::cout << "  BLACK: Dead cell\n";
    std::cout << "  GRAY: Living cell\n";
    std::cout << "\n\n";

    InitWindow(winWidth, winHeight, "Conway's Game of Life");
    SetWindowIcon(LoadImage("icon.png"));

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        updateAndDrawGame();
    }

    CloseWindow();

    return 0;
}

void updateGame(void)
{
    simulation.run();
}

void drawGame(void)
{
    BeginDrawing();

    ClearBackground(windowClearColor);

    simulation.draw();

    if (simulation.isSimulationInProgress())
    {
        DrawText("Simulation: ON", 10, 10, 25, textColor);
    }
    else
    {
        DrawText("Simulation: OFF", 10, 10, 25, textColor);
    }

    EndDrawing();
}

void updateAndDrawGame(void)
{
    updateGame();
    drawGame();
}