#include "Maze.h"
#include "Helpers.h"

#include <iostream>
#include <time.h>

void NewMaze()
{
    int width{};
    int height{};
    int exits{};

    std::cout << "Enter maze width (Odd number 5 to 201): ";
    std::cin >> width;
    Helpers::ClearCin();
    while (width % 2 == 0 || width < 5 || width > 201)
    {
        std::cout << "Invalid width. Please enter an odd number from 5 to 201: ";
        std::cin >> width;
        Helpers::ClearCin();
    }

    std::cout << "Enter maze height (Odd number 5 to 201): ";
    std::cin >> height;
    Helpers::ClearCin();
    while (height % 2 == 0 || height < 5 || height > 201)
    {
        std::cout << "Invalid height. Please enter an odd number from 5 to 201: ";
        std::cin >> height;
        Helpers::ClearCin();
    }

    int maxExits{ width + height - 2 };

    std::cout << "Enter number of exits (0 to " << maxExits << "): ";
    std::cin >> exits;
    Helpers::ClearCin();
    while (exits < 0 || exits > maxExits)
    {
        std::cout << "Invalid exit count. Please enter a number from 0 to " << maxExits << ":";
        std::cin >> exits;
        Helpers::ClearCin();
    }

    std::cout << "\n";

    Maze maze{ width, height, exits, true };

    std::cout << maze;

    if (Helpers::ReceiveYN("Save maze to file? (y/n): "))
        WriteMazeToFile(maze, Helpers::ReceiveFileNameForWrite("File already exists. Overwrite? (y/n): "));
}

void LoadMaze()
{
    std::string fileName{};
    do
        fileName = Helpers::ReceiveFileName();
    while (!Helpers::FileExists(fileName));

    Maze maze{};
    try
    {
        maze = ReadMazeFromFile(fileName);
    }
    catch (std::exception e)
    {
        std::cout << "ERROR: " << e.what() << "\n" << "Enter any key to return to main menu.";
        std::cin;
        Helpers::ClearCin();
        return;
    }

    std::cout << maze;

    if(Helpers::ReceiveYN("Save maze to file? (y/n): "))
        WriteMazeToFile(maze, Helpers::ReceiveFileNameForWrite("File already exists. Overwrite? (y/n): "));
}

void DisplayMainMenu()
{
    system("cls");
    std::cout << "###################################\n";
    std::cout << "#                                 #\n";
    std::cout << "# Maze Generator by Charles Smith #\n";
    std::cout << "#                                 #\n";
    std::cout << "# Commands:                       #\n";
    std::cout << "#                                 #\n";
    std::cout << "# n - generate a brand new maze.  #\n";
    std::cout << "# l - load a maze from file.      #\n";
    std::cout << "# q - quit.                       #\n";
    std::cout << "#                                 #\n";
    std::cout << "###################################\n\n";
    std::cout << "Command: ";
}


int main()
{
    srand((unsigned int)time(NULL));

    char input{};
    bool end{};
    while (!end)
    {
        DisplayMainMenu();
        std::cin >> input;
        Helpers::ClearCin();

        switch (input)
        {
        case 'n':
        case 'N':
            NewMaze();
            break;
        case 'l':
        case 'L':
            LoadMaze();
            break;
        case 'q':
        case'Q':
            end = true;
            break;
        }
    }
}
