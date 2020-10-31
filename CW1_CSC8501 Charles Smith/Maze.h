#pragma once

enum class Cell{Empty = 0, Wall = 1, Start = 2, Exit = 3, Path = 4};
const char CELLCHARS[] = { ' ','#','S','E','O' };

class Maze
{
private:
	int width;
	int height;
	int exits;
	Cell** map;
	void GenerateMaze();
	void GenerateExits();
	const Cell* operator[](int _index) { return map[_index]; }

public:
	Maze(int _width, int _height,int _exits);
	~Maze();

	int Height() const{ return height; }
	int Width() const { return width; }
	Cell At(int _x, int _y) const { return map[_x][_y]; }

	bool InBounds(int _x, int _y) const { return _x > 0 && _x < width - 1 && _y > 0 && _y < height - 1; }
};

void PrintMaze(Maze& _maze);
