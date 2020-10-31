#pragma once

enum class Cell{Empty = 0, Wall = 1, Start = 2, Exit = 3, Path = 4};
const char CELLCHARS[] = { ' ','#','S','E','O' };

struct Coord
{
	int x;
	int y;

	Coord& operator-=(const Coord& _rhs);
	Coord& operator+=(const Coord& _rhs);
};

bool operator==(const Coord& _lhs, const Coord& _rhs);
bool operator!=(const Coord& _lhs, const Coord& _rhs);
Coord operator-(Coord _lhs, const Coord& _rhs);
Coord operator+(Coord _lhs, const Coord& _rhs);

class Maze
{
private:
	int width;
	int height;
	int exitCount;
	Cell** map;

	Coord centre;
	Coord* exits;

	void CreateChildNode();
	void GenerateMaze();
	void GenerateExits();
	void GeneratePaths();
	const Cell* operator[](int _index) { return map[_index]; }

public:
	Maze(int _width, int _height,int _exits);
	~Maze();

	int Height() const { return height; }
	int Width() const { return width; }
	Cell At(int _x, int _y) const { return map[_x][_y]; }

	bool InBounds(int _x, int _y) const { return _x >= 0 && _x < width && _y >= 0 && _y < height; }
};

void PrintMaze(Maze& _maze);
