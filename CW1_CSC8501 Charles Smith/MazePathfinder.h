#pragma once

#include "Coord.h"

#include <map>
#include <vector>
#include <queue>

class Maze;

class MazePathfinder
{
	friend class Maze;
	
	//Private so only Maze can construct this class.
	MazePathfinder(Maze* _maze);

	struct AStarNode
	{
		Coord pos;
		AStarNode* cameFrom;
		int fromStart;
		int fromEnd;
		int totalCost;
	};

	//Functor for comparing a star node's values.
	struct AStarWeightCompare { bool operator()(AStarNode*& _lhs, AStarNode*& _rhs) { return _lhs->totalCost > _rhs->totalCost; } };

	//The maze that owns this pathfinder.
	Maze* maze;

	void Cleanup();
	void AddChildrenToOpenList(AStarNode* _node);
	std::vector<Coord> ConstructPathToFinish(const AStarNode* _start);	

	std::map<Coord,AStarNode*> closedList;
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarWeightCompare> openList;

public:
	std::vector<Coord> operator()(Coord _start);

	//No default constructors
	MazePathfinder() = delete;
	MazePathfinder(MazePathfinder&) = delete;
	MazePathfinder(MazePathfinder&&) = delete;
};