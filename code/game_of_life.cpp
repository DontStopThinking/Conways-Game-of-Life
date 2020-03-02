#include "game_of_life.h"
#include "colors.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "raylib.h"

GameOfLife::GameOfLife(size_t w, size_t h, int cellSize)
{
	std::srand((unsigned)std::time(NULL));

	m_fieldWidth = w;
	m_fieldHeight = h;
	runSimulation = false;
	this->cellSize = cellSize;

	frontGrid = new Cell*[m_fieldWidth / cellSize];
	backGrid = new Cell*[m_fieldHeight / cellSize];
	for (size_t i = 0; i < m_fieldHeight / cellSize; i++)
	{
		frontGrid[i] = new Cell[m_fieldWidth / cellSize];
		backGrid[i] = new Cell[m_fieldWidth / cellSize];
	}
	setGrid();
	//clearGrid();
	//randomizeGrid();
}

GameOfLife::~GameOfLife()
{
	for (size_t i = 0; i < m_fieldHeight / cellSize; i++)
	{
		delete[] frontGrid[i];
	}
	delete[] frontGrid;
}

void GameOfLife::randomizeGrid()
{
	for (size_t i = 0; i < m_fieldWidth / cellSize; i++)
	{
		for (size_t j = 0; j < m_fieldHeight / cellSize; j++)
		{
			backGrid[i][j].size = cellSize;
			if (std::rand() % 100 < 90)
			{
				backGrid[i][j].value = 0;
			}
			else
			{
				backGrid[i][j].value = 1;
			}
		}
	}
	updateFrontGrid();
}

int GameOfLife::getCell(int x, int y)
{
	return frontGrid[x][y].value;
}

void GameOfLife::setCell(int x, int y, int val)
{
	backGrid[x][y].value = val;
}

void GameOfLife::clearGrid()
{
	for (size_t i = 0; i < m_fieldWidth / cellSize; i++)
	{
		for (size_t j = 0; j < m_fieldHeight / cellSize; j++)
		{
			backGrid[i][j].value = 0;
		}
	}
	updateFrontGrid();
}

void GameOfLife::setGrid()
{
	for (size_t i = 0; i < m_fieldWidth / cellSize; i++)
	{
		for (size_t j = 0; j < m_fieldHeight / cellSize; j++)
		{
			backGrid[i][j].value = 1;
		}
	}
	updateFrontGrid();
}

void GameOfLife::updateFrontGrid()
{
	for (size_t i = 0; i < m_fieldWidth / cellSize; i++)
	{
		for (size_t j = 0; j < m_fieldHeight / cellSize; j++)
		{
			frontGrid[i][j] = backGrid[i][j];
		}
	}
}

void GameOfLife::run()
{
	handleInput();
	if (runSimulation)
	{
		updateGrid();
	}
	//draw();
}

void GameOfLife::handleInput()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		if (runSimulation == false)
		{
			std::printf("Simulation run...\n");
			runSimulation = true;
			SetTargetFPS(10);
		}
		else
		{
			std::printf("Simulation paused...\n");
			runSimulation = false;
			SetTargetFPS(60);
		}
	}

	if (!runSimulation)
	{
		int mouseX, mouseY;
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			mouseX = GetMouseX() / cellSize;
			mouseY = GetMouseY() / cellSize;
			//std::printf("mouseX: %d, mouseY: %d\n", mouseX, mouseY);
			backGrid[mouseX][mouseY].value = 1;
			updateFrontGrid();
		}
		else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		{
			mouseX = GetMouseX() / cellSize;
			mouseY = GetMouseY() / cellSize;
			//std::printf("mouseX: %d, mouseY: %d\n", mouseX, mouseY);
			backGrid[mouseX][mouseY].value = 0;
			updateFrontGrid();
		}
		else if (IsKeyPressed(KEY_C))
		{
			clearGrid();
		}
	}
}

void GameOfLife::updateGrid()
{
	if (runSimulation)
	{
		updateFrontGrid();

		for (size_t x = 1; x < (m_fieldWidth / cellSize) - 1; x++)
		{
			for (size_t y = 1; y < (m_fieldHeight / cellSize) - 1; y++)
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
				else if (getCell(x, y) == 0)	// if current cell is dead
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
	//BeginDrawing();

	int x = 0, y = 0;
	for (size_t i = 0; i < m_fieldWidth / cellSize; i++)
	{
		y = 0;
		for (size_t j = 0; j < m_fieldHeight / cellSize; j++)
		{
			if (getCell(i, j) == 1)
			{
				DrawRectangle(x, y, cellSize, cellSize, liveCellColor);
				//DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize / 2, liveCellColor);
			}
			else
			{
				DrawRectangle(x, y, cellSize, cellSize, deadCellColor);
				//DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize / 2, deadCellColor);
			}
			y += cellSize;
		}
		x += cellSize;
	}

	// Draw grid
	for (size_t i = 0; i < m_fieldWidth + 1; i++)
	{
		DrawLine(i * cellSize, 0, i * cellSize, m_fieldHeight + 1, gridOutlineColor);
	}
	for (size_t i = 0; i < m_fieldHeight + 1; i++)
	{
		DrawLine(0, i * cellSize, m_fieldWidth + 1, i * cellSize, gridOutlineColor);
	}

	//EndDrawing();
}

bool GameOfLife::isSimulationInProgress()
{
	return runSimulation;
}