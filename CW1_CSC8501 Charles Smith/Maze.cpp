#include <iostream>
#include <algorithm>
#include <vector>
#include "Maze.h"
#include <queue>

struct FrontierCell
{
	Coord loc;
	Coord connector;
};

struct AStarNode
{
	Coord pos;
	AStarNode* cameFrom;
	int fromStart;
	int fromEnd;
	int total;
};

bool operator==(const AStarNode& _lhs, const AStarNode& _rhs)
{
	return (_lhs.pos == _rhs.pos);
}

struct AStarCompare
{
	bool operator()(AStarNode*& lhs, AStarNode*& rhs) { return lhs->total > rhs->total; }
};

void Maze::GeneratePaths()
{
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
			if (curr->pos == centre || map[curr->pos.x][curr->pos.y] == Cell::Path)
			{
				endNode = curr;
				break;
			}

			const Coord childModifiers[]{ {1,0} ,{0,1},{-1,0},{0,-1} };
			for (int i = 0; i < 4; i++)
			{
				Coord childPos = curr->pos + childModifiers[i];

				if (!InBounds(childPos.x, childPos.y) || map[childPos.x][childPos.y] == Cell::Wall)
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
				map[endNode->pos.x][endNode->pos.y] = Cell::Path;
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

	exits = new Coord[exitCount];

	for (int i = 0; i < exitCount; i++)
	{
		exits[i] = possibleExits[i];
		map[possibleExits[i].x][possibleExits[i].y] = Cell::Exit;
	}
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

		Coord loc = currCell.loc;

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

	centre = {width / 2,height / 2 };
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			map[centre.x + x][centre.y + y] = Cell::Empty;
	map[centre.x][centre.y] = Cell::Start;
}

Maze::Maze(int _width, int _height, int _exits)
{

	height = _height;
	width = _width;
	exitCount = _exits;

	map = new Cell* [width];

	for (int i = 0; i < width; i++)
		map[i] = new Cell[height] {};

	GenerateMaze();
	GenerateExits();
	GeneratePaths();
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