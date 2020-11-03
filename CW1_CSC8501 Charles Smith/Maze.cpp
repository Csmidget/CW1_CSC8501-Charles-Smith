#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include "Maze.h"

void Maze::GeneratePaths()
{
	struct AStarNode
	{
		Coord pos;
		AStarNode* cameFrom;
		int fromStart;
		int fromEnd;
		int totalCost;
	};

	struct AStarCompare
	{
		bool operator()(AStarNode*& lhs, AStarNode*& rhs) { return lhs->totalCost > rhs->totalCost; }
	};

	std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare> openNodes{};
	std::vector<AStarNode*> closedNodes{};

	for (int i = 0; i < exitCount; i++)
	{
		Coord start = exits[i];

		AStarNode* endNode{ nullptr };

		openNodes.push(new AStarNode{ start,nullptr,0,0,0 });

		while (openNodes.size() > 0)
		{
			AStarNode* curr = openNodes.top();
			openNodes.pop();

			if (std::find_if(closedNodes.begin(), closedNodes.end(), [curr](const AStarNode* val) { return val->pos == curr->pos; }) != closedNodes.end())
			{
				delete curr;
				continue;
			}

			closedNodes.push_back(curr);

			//Reusing existing paths saves on wasting time retreading the same ground
			if (curr->pos == centre || (*this)[curr->pos.x][curr->pos.y] == Cell::Path)
			{
				endNode = curr;
				break;
			}

			const Coord childModifiers[]{ {1,0} ,{0,1},{-1,0},{0,-1} };
			for (int i = 0; i < 4; i++)
			{
				Coord childPos = curr->pos + childModifiers[i];

				if (!InBounds(childPos.x, childPos.y) || (*this)[childPos.x][childPos.y] == Cell::Wall)
					continue;

				int fromStart = curr->fromStart + 1;
				int fromEnd = abs(childPos.x - centre.x) + abs(childPos.y - centre.y);
				openNodes.push(new AStarNode{ childPos,curr,fromStart,fromEnd,fromStart + fromEnd });
			}
		}

		if (endNode != nullptr)
		{
			endNode = endNode->cameFrom;
			while (endNode->cameFrom != nullptr)
			{
				(*this)[endNode->pos.x][endNode->pos.y] = Cell::Path;
				endNode = endNode->cameFrom;				
			}
		}

		while (openNodes.size() > 0)
		{
			delete openNodes.top();
			openNodes.pop();
		}

		for (int i = 0; i < closedNodes.size(); i++)
			delete closedNodes[i];
		closedNodes.clear();
	}
}

void Maze::GenerateExits() 
{
	//This solution ensures that no exits will overlap with one another and is of known complexity ( O(n) )
	std::vector<Coord> possibleExits;
	possibleExits.reserve(width + height - 2);

	for (int i = 1; i < width / 2 + 1; i++)
		possibleExits.insert(possibleExits.end(), { {i*2-1,0} , {i*2-1,height - 1} });
	for (int i = 1; i < height / 2 + 1; i++)
		possibleExits.insert(possibleExits.end(), { {0,i*2-1} , {width- 1 ,i*2-1} });

	std::random_shuffle(possibleExits.begin(), possibleExits.end());

	exitCount = std::min(exitCount, (int)possibleExits.size());

	for (int i = 0; i < exitCount; i++)
	{
		exits[i] = possibleExits[i];
		(*this)[possibleExits[i].x][possibleExits[i].y] = Cell::Exit;
	}
}

void Maze::GenerateMaze()
{
	struct FrontierCell
	{
		Coord loc;
		Coord connector;
	};

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			(*this)[x][y] = Cell::Wall;
	(*this)[1][1] = Cell::Empty;

	std::vector<FrontierCell> frontierCells{};

	frontierCells.push_back({ {3,1},{2,1} });
	frontierCells.push_back({ {1,3},{1,2} });

	while (frontierCells.size() > 0)
	{
		int pos = rand() % frontierCells.size();

		FrontierCell currCell = frontierCells[pos];
		frontierCells.erase(frontierCells.begin() + pos);

		if ((*this)[currCell.loc.x][currCell.loc.y] != Cell::Wall) continue;

		Coord loc = currCell.loc;

		(*this)[loc.x][loc.y] = Cell::Empty;
		(*this)[currCell.connector.x][currCell.connector.y] = Cell::Empty;

		if (loc.x + 2 < width - 1 && (*this)[loc.x + 2][loc.y] == Cell::Wall)
			frontierCells.push_back({ {loc.x + 2, loc.y},{loc.x + 1, loc.y} });

		if (loc.x - 2 > 0 && (*this)[loc.x - 2][loc.y] == Cell::Wall)
			frontierCells.push_back({ {loc.x - 2, loc.y},{loc.x - 1, loc.y} });

		if (loc.y + 2 < height - 1 && (*this)[loc.x][loc.y + 2] == Cell::Wall)
			frontierCells.push_back({ {loc.x, loc.y + 2},{loc.x, loc.y + 1} });

		if (loc.y - 2 > 0 && (*this)[loc.x][loc.y - 2] == Cell::Wall)
			frontierCells.push_back({ {loc.x, loc.y - 2},{loc.x, loc.y - 1} });
	}

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			(*this)[centre.x + x][centre.y + y] = Cell::Empty;
	(*this)[centre.x][centre.y] = Cell::Start;
}

Maze::Maze(int _width, int _height, int _exits, bool _generate)
{

	height = _height;
	width = _width;
	exitCount = _exits;
	centre = { width / 2,height / 2 };
	map = new Cell [width * height];

	exits = new Coord[width + height - 2];

	if (_generate)
	{
		GenerateMaze();
		GenerateExits();
		GeneratePaths();
	}
}

//Copy Constructor
Maze::Maze(const Maze& _maze)
{
	height = _maze.height;
	width = _maze.width;
	exitCount = _maze.exitCount;
	centre = _maze.centre;

	map = new Cell [width * height];

	memcpy(map, _maze.map,sizeof(Cell) * width * height);

	exits = new Coord[width + height - 2];
	memcpy(exits, _maze.exits, sizeof(Coord) * (width + height - 2));
}

//Move constructor
Maze::Maze(Maze&& _maze) noexcept
{
	width = _maze.width;
	height = _maze.height;
	exitCount = _maze.exitCount;
	centre = _maze.centre;
	map = _maze.map;
	exits = _maze.exits;

	_maze.map = nullptr;
	_maze.exits = nullptr;
}

Maze::~Maze()
{
	delete[] map;
	delete[] exits;
}

void PrintMaze(const Maze& _maze)
{
	for (int y = 0; y < _maze.Height(); y++)
	{
		for (int x = 0; x < _maze.Width(); x++)
			std::cout << (CELLCHARS[(int)_maze.At(x,y)]);
		std::cout << "\n";
	}
	std::cout << "\n";
}

void WriteMazeToFile(const Maze& _maze)
{
	std::string fileName{};
	std::cout << "Enter file name, including extension: ";
	std::cin >> fileName;

	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		std::cout << "Invalid name entered, try again: ";
		std::cin >> fileName;
	}

	std::ofstream file;
	file.open(fileName);
	file.clear();

	for (int x = 0; x < _maze.Width(); x++)
		file << CELLCHARS[(int)_maze.At(x, 0)];

	for (int y = 1; y < _maze.Height(); y++)
	{
		file << "\n";
		for (int x = 0; x < _maze.Width(); x++)
			file << CELLCHARS[(int)_maze.At(x,y)];
	}

	file.close();
}

Cell CharToCell(char _char)
{
	for (int i = 0; i < CELLTYPECOUNT; i++)
		if (CELLCHARS[i] == _char)
			return (Cell)i;

	return Cell::Invalid;
}

//Factory function to generate maze from file.
Maze ReadMazeFromFile()
{
	std::string fileName{};
	std::cout << "Enter file name, including extension: ";
	std::cin >> fileName;

	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		std::cout << "Invalid name entered, try again: ";
		std::cin >> fileName;
	} 

	std::ifstream file;
	std::string line;
	file.open(fileName);

	if (file.fail())
		throw std::exception("Unable to open file.");

	getline(file, line);

	int width = line.size();
	int height{ 1 };

	while (!file.eof())
	{
		std::getline(file, line);
		height++;
	}
	Maze maze = Maze(width, height, 0, false);

	file.clear();
	file.seekg(0);

	char inChar;
	int currLine{ 0 };
	int currChar{ 0 };

	for (int y = 0; y < height; y++)
	{
		getline(file, line);

		for (int x = 0; x < line.size(); x++)
		{
			Cell cell = CharToCell(line[x]);
			maze[x][y] = cell;

			switch (cell)
			{
			case Cell::Exit:
				maze.exitCount++;
				maze.exits[maze.exitCount - 1] = { x,y };
				break;
			case Cell::Start:
				maze.centre = { x,y };
				break;
			}
		}
	}

	file.close();

	return maze;
}