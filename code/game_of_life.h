#pragma once
#include "cell.h"

class GameOfLife
{
private:

    size_t m_fieldWidth;
    size_t m_fieldHeight;
    bool m_runSimulation = false;
    int m_cellSize;

    Cell **m_backGrid;    // used as a backbuffer to update all cells at once before drawing
    Cell **m_frontGrid;   // actually used to draw the cells

    // private methods
    void updateGrid();                   // updates the back grid cells as per the rules of Conway's Game of Life
    void randomizeGrid();
    int getCell(int x, int y);	         // returns the front grid cell value at (x, y)
    void setCell(int x, int y, int val); // sets the back grid cell at (x, y) to val
    void handleInput();
    void updatem_frontGrid();	         // syncs the front grid with the back grid
    void clearGrid();                    // sets all cells value to 0
    void setGrid();	                     // sets all cells to 1

public:

    GameOfLife(size_t w, size_t h, int m_cellSize);
    ~GameOfLife();

    void run();
    bool isSimulationInProgress();
    void draw();
};