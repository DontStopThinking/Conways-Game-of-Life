#include "game_of_life.h"
#include "colors.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "raylib.h"

GameOfLife::GameOfLife(size_t w, size_t h, int m_cellSize)
{
    std::srand((unsigned)std::time(NULL));

    m_fieldWidth = w;
    m_fieldHeight = h;
    m_runSimulation = false;
    this->m_cellSize = m_cellSize;

    m_frontGrid = new Cell*[m_fieldWidth / m_cellSize];
    m_backGrid = new Cell*[m_fieldHeight / m_cellSize];
    for (size_t i = 0; i < m_fieldHeight / m_cellSize; i++)
    {
        m_frontGrid[i] = new Cell[m_fieldWidth / m_cellSize];
        m_backGrid[i] = new Cell[m_fieldWidth / m_cellSize];
    }
    setGrid();
    //clearGrid();
    //randomizeGrid();
}

GameOfLife::~GameOfLife()
{
    for (size_t i = 0; i < m_fieldHeight / m_cellSize; i++)
    {
        delete[] m_frontGrid[i];
    }
    delete[] m_frontGrid;
}

void GameOfLife::randomizeGrid()
{
    for (size_t i = 0; i < m_fieldWidth / m_cellSize; i++)
    {
        for (size_t j = 0; j < m_fieldHeight / m_cellSize; j++)
        {
            m_backGrid[i][j].size = m_cellSize;
            if (std::rand() % 100 < 90)
            {
                m_backGrid[i][j].value = 0;
            }
            else
            {
                m_backGrid[i][j].value = 1;
            }
        }
    }
    updatem_frontGrid();
}

int GameOfLife::getCell(int x, int y)
{
    return m_frontGrid[x][y].value;
}

void GameOfLife::setCell(int x, int y, int val)
{
    m_backGrid[x][y].value = val;
}

void GameOfLife::clearGrid()
{
    for (size_t i = 0; i < m_fieldWidth / m_cellSize; i++)
    {
        for (size_t j = 0; j < m_fieldHeight / m_cellSize; j++)
        {
            m_backGrid[i][j].value = 0;
        }
    }
    updatem_frontGrid();
}

void GameOfLife::setGrid()
{
    for (size_t i = 0; i < m_fieldWidth / m_cellSize; i++)
    {
        for (size_t j = 0; j < m_fieldHeight / m_cellSize; j++)
        {
            m_backGrid[i][j].value = 1;
        }
    }
    updatem_frontGrid();
}

void GameOfLife::updatem_frontGrid()
{
    for (size_t i = 0; i < m_fieldWidth / m_cellSize; i++)
    {
        for (size_t j = 0; j < m_fieldHeight / m_cellSize; j++)
        {
            m_frontGrid[i][j] = m_backGrid[i][j];
        }
    }
}

void GameOfLife::run()
{
    handleInput();
    if (m_runSimulation)
    {
        updateGrid();
    }
}

void GameOfLife::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        if (m_runSimulation == false)
        {
            std::printf("Simulation run...\n");
            m_runSimulation = true;
            SetTargetFPS(10);
        }
        else
        {
            std::printf("Simulation paused...\n");
            m_runSimulation = false;
            SetTargetFPS(60);
        }
    }

    if (!m_runSimulation)
    {
        int mouseX, mouseY;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            mouseX = GetMouseX() / m_cellSize;
            mouseY = GetMouseY() / m_cellSize;
            //std::printf("mouseX: %d, mouseY: %d\n", mouseX, mouseY);
            m_backGrid[mouseX][mouseY].value = 1;
            updatem_frontGrid();
        }
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            mouseX = GetMouseX() / m_cellSize;
            mouseY = GetMouseY() / m_cellSize;
            //std::printf("mouseX: %d, mouseY: %d\n", mouseX, mouseY);
            m_backGrid[mouseX][mouseY].value = 0;
            updatem_frontGrid();
        }
        else if (IsKeyPressed(KEY_C))
        {
            clearGrid();
        }
    }
}

void GameOfLife::updateGrid()
{
    if (m_runSimulation)
    {
        updatem_frontGrid();

        for (size_t x = 1; x < (m_fieldWidth / m_cellSize) - 1; x++)
        {
            for (size_t y = 1; y < (m_fieldHeight / m_cellSize) - 1; y++)
            {
                int east = getCell(x - 1, y);
                int west = getCell(x + 1, y);
                int north = getCell(x, y - 1);
                int south = getCell(x, y + 1);
                int northEast = getCell(x + 1, y - 1);
                int northWest = getCell(x - 1, y - 1);
                int southEast = getCell(x + 1, y + 1);
                int southWest = getCell(x - 1, y + 1);
                int totalNeighboursCount = east + west + north + south + northEast + northWest + southEast + southWest;

                if (getCell(x, y) == 1)	// if current cell is alive
                {
                    if (totalNeighboursCount < 2)
                    {
                        setCell(x, y, 0);
                    }
                    else if (totalNeighboursCount == 2 || totalNeighboursCount == 3)
                    {
                        setCell(x, y, 1);
                    }
                    else if (totalNeighboursCount > 3)
                    {
                        setCell(x, y, 0);
                    }
                }
                else if (getCell(x, y) == 0)    // if current cell is dead
                {
                    if (totalNeighboursCount == 3)
                    {
                        setCell(x, y, 1);
                    }
                }
            }
        }
    }
}

void GameOfLife::draw()
{
    int x = 0, y = 0;
    for (size_t i = 0; i < m_fieldWidth / m_cellSize; i++)
    {
        y = 0;
        for (size_t j = 0; j < m_fieldHeight / m_cellSize; j++)
        {
            if (getCell(i, j) == 1)
            {
                DrawRectangle(x, y, m_cellSize, m_cellSize, liveCellColor);
                //DrawCircle(x + m_cellSize / 2, y + m_cellSize / 2, m_cellSize / 2, liveCellColor);
            }
            else
            {
                DrawRectangle(x, y, m_cellSize, m_cellSize, deadCellColor);
                //DrawCircle(x + m_cellSize / 2, y + m_cellSize / 2, m_cellSize / 2, deadCellColor);
            }
            y += m_cellSize;
        }
        x += m_cellSize;
    }

    // Draws grid lines
    for (size_t i = 0; i < m_fieldWidth + 1; i++)
    {
        DrawLine(i * m_cellSize, 0, i * m_cellSize, m_fieldHeight + 1, gridOutlineColor);
    }
    for (size_t i = 0; i < m_fieldHeight + 1; i++)
    {
        DrawLine(0, i * m_cellSize, m_fieldWidth + 1, i * m_cellSize, gridOutlineColor);
    }
}

bool GameOfLife::isSimulationInProgress()
{
    return m_runSimulation;
}