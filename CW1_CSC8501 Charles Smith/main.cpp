// CW1_CSC8501 Charles Smith.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "Maze.h"

int main()
{
    int width = 75;
    int height = 75;
    int exits = 3;

    srand(time(NULL));

    Maze maze(width, height,exits);

    PrintMaze(maze);
}
