#include <iostream>
#include <algorithm>
#include <vector>
#include "Maze.h"

struct Coords
{
	int x;
	int y;
};

//Stores a cell on the frontier of maze gen algorithm along with the cell that connects it to the generated maze.
struct FrontierCell
{
	Coords loc;
	Coords connector;
};

void Maze::GenerateExits() 
{
	std::vector<Coords> possibleExits{};

	for (int i = 1; i < width / 2 + 1; i++)
		possibleExits.insert(possibleExits.end(), { {i*2-1,0} , {i*2-1,height - 1} });
	for (int i = 1; i < height / 2 + 1; i++)
		possibleExits.insert(possibleExits.end(), { {0,i*2-1} , {width- 1 ,i*2-1} });

	std::random_shuffle(possibleExits.begin(), possibleExits.end());

	exits = std::min(exits, (int)possibleExits.size());

	for (int i = 0; i < exits; i++)
		map[possibleExits[i].x][possibleExits[i].y] = Cell::Exit;

}

void Maze::GenerateMaze()
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			map[x][y] = Cell::Wall;
	map[1][1] = Cell::Empty;

	std::vector<FrontierCell> frontierCells{};

	frontierCells.push_back({ {3,1},{2,1} });
	frontierCells.push_back({ {1,3},{1,2} });

	while (frontierCells.size() > 0)
	{
		int pos = rand() % frontierCells.size();

		FrontierCell currCell = frontierCells[pos];
		frontierCells.erase(frontierCells.begin() + pos);

		if (map[currCell.loc.x][currCell.loc.y] != Cell::Wall) continue;

		Coords loc = currCell.loc;

		map[loc.x][loc.y] = Cell::Empty;
		map[currCell.connector.x][currCell.connector.y] = Cell::Empty;

		if (loc.x + 2 < width - 1 && map[loc.x + 2][loc.y] == Cell::Wall)
			frontierCells.push_back({ {loc.x + 2, loc.y},{loc.x + 1, loc.y} });

		if (loc.x - 2 > 0 && map[loc.x - 2][loc.y] == Cell::Wall)
			frontierCells.push_back({ {loc.x - 2, loc.y},{loc.x - 1, loc.y} });

		if (loc.y + 2 < height - 1 && map[loc.x][loc.y + 2] == Cell::Wall)
			frontierCells.push_back({ {loc.x, loc.y + 2},{loc.x, loc.y + 1} });

		if (loc.y - 2 > 0 && map[loc.x][loc.y - 2] == Cell::Wall)
			frontierCells.push_back({ {loc.x, loc.y - 2},{loc.x, loc.y - 1} });
	}

	Coords mid{ width / 2,height / 2 };
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			map[mid.x + x][mid.y + y] = Cell::Empty;

	GenerateExits();

	map[mid.x][mid.y] = Cell::Start;
}

Maze::Maze(int _width, int _height, int _exits)
{
	assert(_width >= 5);
	assert(_height >= 5);

	height = _height;
	width = _width;
	exits = _exits;

	map = new Cell* [width];

	for (int i = 0; i < width; i++)
		map[i] = new Cell[height] {};

	GenerateMaze();
}

Maze::~Maze()
{
	for (int i = 0; i < width; i++)
		delete[] map[i];

	delete[] map;
}

void PrintMaze(Maze& _maze)
{
	for (int y = 0; y < _maze.Height(); y++)
	{
		for (int x = 0; x < _maze.Width(); x++)
			std::cout << (CELLCHARS[(int)_maze.At(x,y)]);

		std::cout << "\n";
	}
	std::cout << "\n";
}