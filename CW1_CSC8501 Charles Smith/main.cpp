// CW1_CSC8501 Charles Smith.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "Maze.h"
#include <Windows.h>

//Clear the Cin stream in case a bunch of junk was entered.
void ClearCin()
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

int main()
{
    srand(time(NULL));

    int width{};
    int height{};
    int exits{};

    std::cout << "Enter maze width (Odd number 5 to 201): ";
    std::cin >> width;
    while (width % 2 == 0 || width < 0 || width > 201)
    {
        std::cout << "Invalid width. Please enter an odd number from 5 to 201: ";
        std::cin >> width;
        ClearCin();
    }

    std::cout << "Enter maze height (Odd number 5 to 201): ";
    std::cin >> height;
    while (height % 2 == 0 || height < 0 || height > 201)
    {
        std::cout << "Invalid height. Please enter an odd number from 5 to 201: ";
        std::cin >> height;
        ClearCin();
    }

    int maxExits = width + height - 2;

    std::cout << "Enter number of exits (0 to "<< maxExits << "): ";
    std::cin >> exits;
    while (exits < 0 || exits > maxExits)
    {
        std::cout << "Invalid exit count. Please enter a number from 0 to " << maxExits << ":";
        std::cin >> exits;
        ClearCin();
    }

    std::cout << "\n";
    for (int i = 0; i < 100000; i++)
    {
        std::cout << i << "\n";
        Maze maze(width, height, exits);
    }
    

   // PrintMaze(maze);
}
