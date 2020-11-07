#include "MazePathfinder.h"

#include "Maze.h"

MazePathfinder::MazePathfinder(Maze* _maze)
{
	maze = _maze;
}

//Return to default state ready for next request.
void MazePathfinder::Cleanup()
{
	while (openList.size() > 0)
	{
		delete openList.top();
		openList.pop();
	}

	for (auto i{ closedList.begin() }; i != closedList.end(); i++)
		delete i->second;
	closedList.clear();
}

void MazePathfinder::AddChildrenToOpenList(AStarNode* _node)
{
	const Coord childModifiers[]{ {1,0} ,{0,1},{-1,0},{0,-1} };
	for (size_t i{}; i < 4; i++)
	{
		Coord childPos{ _node->pos + childModifiers[i] };

		if (!maze->InBounds(childPos) || maze->At(childPos) == Cell::Wall)
			continue;

		int fromStart{ _node->fromStart + 1 };
		int fromEnd{ abs(childPos.x - maze->Centre().x) + abs(childPos.y - maze->Centre().y) };
		openList.push(new AStarNode{ childPos,_node,fromStart,fromEnd,fromStart + fromEnd });
	}
}

std::vector<Coord> MazePathfinder::ConstructPathToFinish(const AStarNode* _start)
{
	std::vector<Coord> path;
	auto curr{_start};

	if (curr != nullptr)
		while (curr->cameFrom != nullptr)
		{
			path.push_back(curr->pos);
			curr = curr->cameFrom;
		}

	return path;
}

std::vector<Coord> MazePathfinder::operator()(Coord _from)
{
	openList.push(new AStarNode{ _from,nullptr,0,0,0 });

	AStarNode* endNode{nullptr};

	while (openList.size() > 0)
	{
		AStarNode* curr{openList.top()};
		openList.pop();
				
		if (closedList.find(curr->pos) != closedList.end())
		{
			delete curr;
			continue;
		}

		closedList[curr->pos] = curr;

		//Reusing existing paths saves on wasting time retreading the same ground
		if (curr->pos == maze->Centre() || maze->At(curr->pos) == Cell::Path)
		{
			endNode = curr->cameFrom;
			break;
		}

		AddChildrenToOpenList(curr);
	}

	std::vector<Coord> path{ ConstructPathToFinish(endNode) };

	Cleanup();

	return path;
}