# Conway's Game of Life
Conway's Game of Life simulation made using C++ and Raylib

![](https://raw.githubusercontent.com/ShantanuBalse/Conways-Game-of-Life/master/screenshots/conway2.gif)

## Rules:
  On each iteration, check all the 8 neighbors of each cell.
  Then, for a living cell, if the cell:
  - has less than 2 living neighbors, it dies (underpopulation)
  - has 2 or 3 living neighbors, it survives
  - has more than 3 living neighbors, it dies (overpopulation)
  
  For a dead cell, if the cell:
  - has 3 living neighbors, it becomes a live cell

## Basic Controls
  - Spacebar: start/pause simulation
  - C: clear the field when paused
  - Left-click: place a cell when paused
  - Right-click: remove a cell when paused
  
## Colors
  - Black: dead cell
  - White: live cell
