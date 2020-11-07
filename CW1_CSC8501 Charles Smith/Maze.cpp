#include "Maze.h"

#include "Helpers.h"

#include <iostream>
#include <fstream>

//Blank maze
Maze::Maze() : pathfinder{ this }
{
	height = 0;
	width = 0;
	exitCount = 0;
	centre = {};
	map = new Cell[0];
	exits = new Coord[0];
}

Maze::Maze(int _width, int _height, int _exits, bool _generate) : pathfinder{ this }
{

	height = _height;
	width = _width;
	exitCount = _exits;
	centre = { width / 2,height / 2 };
	map = new Cell[width * height];

	exits = new Coord[width + height - 2];

	if (_generate)
	{
		GenerateMaze();
		GenerateExits();
		GeneratePaths();
	}
}

//Copy Constructor
Maze::Maze(const Maze& _maze) : pathfinder{ this }
{
	map = nullptr;
	exits = nullptr;
	*this = _maze;

	height = _maze.height;
	width = _maze.width;
	exitCount = _maze.exitCount;
	centre = _maze.centre;

	map = new Cell[width * height];

	memcpy(map, _maze.map, sizeof(Cell) * width * height);

	exits = new Coord[width + height - 2];
	memcpy(exits, _maze.exits, sizeof(Coord) * (width + height - 2));
}

//Move constructor
Maze::Maze(Maze&& _maze) noexcept : pathfinder{ this }
{
	map = nullptr;
	exits = nullptr;
	*this = _maze;

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

Maze& Maze::operator=(const Maze& _maze)
{
	delete[] map;

	height = _maze.height;
	width = _maze.width;
	exitCount = _maze.exitCount;
	centre = _maze.centre;

	map = new Cell[width * height];
	memcpy(map, _maze.map, sizeof(Cell) * width * height);

	exits = new Coord[width + height - 2];
	memcpy(exits, _maze.exits, sizeof(Coord) * (width + height - 2));

	return *this;
}

Maze& Maze::operator=(Maze&& _maze) noexcept
{
	width = _maze.width;
	height = _maze.height;
	exitCount = _maze.exitCount;
	centre = _maze.centre;

	Cell* oldMap = map;
	map = _maze.map;
	_maze.map = oldMap;

	Coord* oldExits = exits;
	exits = _maze.exits;
	_maze.exits = oldExits;

	return *this;
}

void Maze::GeneratePaths()
{
	for (int i{}; i < exitCount; i++)
	{
		auto path{pathfinder(exits[i])};
		for (size_t j{}; j < path.size(); j++)
			(*this)[path[j]] = Cell::Path;
	}
}

void Maze::GenerateExits() 
{
	//This solution ensures that no exits will overlap with one another and is of known complexity ( O(n) )
	std::vector<Coord> possibleExits{};
	possibleExits.reserve(width + height - 2);

	for (int i{2}; i < width; i += 2)
		possibleExits.insert(possibleExits.end(), { {i - 1,0} , {i - 1,height - 1} });
	for (int i{2}; i < height; i += 2)
		possibleExits.insert(possibleExits.end(), { {0,i - 1} , {width - 1 ,i - 1} });

	std::random_shuffle(possibleExits.begin(), possibleExits.end());

	exitCount = std::min(exitCount, (int)possibleExits.size());

	for (int i{}; i < exitCount; i++)
	{
		exits[i] = possibleExits[i];
		(*this)[possibleExits[i].x][possibleExits[i].y] = Cell::Exit;
	}
}

//Randomized Prim's algorithm for maze generation.
void Maze::GenerateMaze()
{
	struct FrontierCell
	{
		Coord loc;
		Coord connector;
	};

	for (int y{}; y < height; y++)
		for (int x{}; x < width; x++)
			(*this)[x][y] = Cell::Wall;
	(*this)[1][1] = Cell::Empty;

	std::vector<FrontierCell> frontierCells{};

	frontierCells.push_back({ {3,1},{2,1} });
	frontierCells.push_back({ {1,3},{1,2} });

	while (frontierCells.size() > 0)
	{
		size_t pos{ rand() % frontierCells.size() };

		FrontierCell currCell{ frontierCells[pos] };
		frontierCells.erase(frontierCells.begin() + pos);

		if ((*this)[currCell.loc.x][currCell.loc.y] != Cell::Wall) continue;

		Coord loc{currCell.loc};

		(*this)[loc] = Cell::Empty;
		(*this)[currCell.connector] = Cell::Empty;

		if (loc.x + 2 < width - 1 && (*this)[loc + Coord{ 2,0 }] == Cell::Wall)
			frontierCells.push_back({ loc + Coord{2,0},loc + Coord{1,0} });

		if (loc.x - 2 > 0 && (*this)[loc - Coord{ 2,0 }] == Cell::Wall)
			frontierCells.push_back({ loc - Coord{2,0},loc - Coord{1,0} });

		if (loc.y + 2 < height - 1 && (*this)[loc + Coord{ 0,2 }] == Cell::Wall)
			frontierCells.push_back({ loc + Coord{0,2},loc + Coord{0,1} });

		if (loc.y - 2 > 0 && (*this)[loc - Coord{ 0,2 }] == Cell::Wall)
			frontierCells.push_back({ loc - Coord{0,2},loc - Coord{0,1} });
	}

	for (int x{-1}; x <= 1; x++)
		for (int y{-1}; y <= 1; y++)
			(*this)[centre + Coord{ x,y }] = Cell::Empty;
	(*this)[centre] = Cell::Start;
}

std::ostream& operator<<(std::ostream& _stream, const Maze& _maze)
{
	for (int y{}; y < _maze.Height(); y++)
	{
		for (int x{}; x < _maze.Width(); x++)
			_stream << (CELLCHARS[(int)_maze.At(x,y)]);
		_stream << "\n";
	}
	_stream << "\n";
	return _stream;
}

void WriteMazeToFile(const Maze& _maze, const std::string& _fileName)
{
	std::ofstream file;
	file.open(_fileName);

	file << _maze;

	file.close();
}

//Factory function to generate maze from file.
Maze ReadMazeFromFile(const std::string& _fileName)
{
	std::ifstream file{};
	std::string line{};

	file.open(_fileName);

	if (file.fail())
		throw std::exception("Unable to open file.");

	getline(file, line);

	int width{(int)line.size()};
	int height{ 1 };

	while (!file.eof())
	{
		std::getline(file, line);
		if (line.size() != 0)
			height++;
	}
	Maze maze{Maze(width, height, 0, false)};

	file.clear();
	file.seekg(0);

	char inChar{};
	int currLine{};
	int currChar{};

	for (int y{}; y < height; y++)
	{
		getline(file, line);

		if (line.size() == 0)
			throw std::exception("Provided file is not valid maze.");

		for (int x{}; x < (int)line.size(); x++)
		{
			Cell cell{CharToCell(line[x])};
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