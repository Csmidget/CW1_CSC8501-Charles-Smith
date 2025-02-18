#pragma once

#include "Coord.h"
#include "Cell.h"
#include "MazePathfinder.h"
#include <string>

class Maze
{
private:
	int width;
	int height;
	int exitCount;
	Coord centre;

	Cell* map;
	Coord* exits;

	MazePathfinder pathfinder;

	void GenerateMaze();
	void GenerateExits();
	void GeneratePaths();
	Cell* operator[](size_t _index) { return &map[_index * height]; }
	Cell& operator[](Coord _xy) { return map[_xy.x * height + _xy.y]; }

	friend Maze ReadMazeFromFile(const std::string& _fileName);

public:
	Maze();
	Maze(int _width, int _height,int _exits, bool _generate);
	Maze(const Maze&); // Copy constructor
	Maze(Maze&&) noexcept; //Move constructor
	~Maze();

	Maze& operator=(const Maze& _maze); //Copy Assignment
	Maze& operator=(Maze&& _maze) noexcept; //Move Assignment

	int Height() const { return height; }
	int Width() const { return width; }
	Coord Centre() const { return centre; }
	Cell At(size_t _x, size_t _y) const { return map[_x* height + _y]; }
	Cell At(Coord _xy) const { return map[_xy.x * height + _xy.y]; }

	bool InBounds(Coord _xy) const { return _xy.x >= 0 && _xy.x < width&& _xy.y >= 0 && _xy.y < height; }
};

std::ostream& operator<<(std::ostream& _stream, const Maze& _maze);
void WriteMazeToFile(const Maze& _maze, const std::string& _fileName);
Maze ReadMazeFromFile(const std::string& _fileName);