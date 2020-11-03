#pragma once

enum class Cell{Empty = 0, Wall = 1, Start = 2, Exit = 3, Path = 4,Invalid = 5};
const int CELLTYPECOUNT = 6;
const char CELLCHARS[CELLTYPECOUNT] = { ' ','#','S','E','o','?'};

struct Coord
{
	int x;
	int y;

	Coord& operator+=(const Coord& _rhs);
};

bool operator==(const Coord& _lhs, const Coord& _rhs);
bool operator!=(const Coord& _lhs, const Coord& _rhs);
Coord operator+(Coord _lhs, const Coord& _rhs);

class Maze
{
private:
	int width;
	int height;
	int exitCount;
	Coord centre;

	Cell* map;
	Coord* exits;

	void GenerateMaze();
	void GenerateExits();
	void GeneratePaths();
	Cell* operator[](int _index) { return &map[_index * width]; }

	friend Maze ReadMazeFromFile();

public:
	Maze(int _width, int _height,int _exits, bool _generate);
	Maze(const Maze&); // Copy constructor
	Maze(Maze&&) noexcept; //Move constructor
	~Maze();

	int Height() const { return height; }
	int Width() const { return width; }
	Cell At(int _x, int _y) const { return map[_x* width + _y]; }

	bool InBounds(int _x, int _y) const { return _x >= 0 && _x < width && _y >= 0 && _y < height; }
};

Cell CharToCell(char _char);
void PrintMaze(const Maze& _maze);
void WriteMazeToFile(const Maze& _maze);
Maze ReadMazeFromFile();