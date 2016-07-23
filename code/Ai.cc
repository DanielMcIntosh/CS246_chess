#include "Ai.h"
#include "Move.h"
#include "Game.h"

#include <vector>
#include <utility>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

const int priorityMasks[4] = {0b0001, 0b0111, 0b1111, 0b1111};

Move Ai::getMove(Game *g)
{
	cout << "AI test 0" << endl;
	cout << "lvl = " << lvl << endl;
	vector<Move> topMoves{};
	int maxPriority = 0;
	int ind = 0;
	for (auto i: this->myPieces)
	{

		cout << "AI test 0." << ind << endl;
		cout << "testing moves available to piece at " << i.first << ", " << i.second << endl;
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				if (i.first == x && i.second == y)
				{
					continue;
				}
				Move curMove(i, make_pair(x, y), colour ? 'q' : 'Q', colour);
				int curPriority = g->tryMove(curMove, priorityMasks[this->lvl - 1]);
				curPriority &= priorityMasks[this->lvl - 1];
				if (curPriority > maxPriority)
				{
					topMoves.clear();
					maxPriority = curPriority;
				}
				if (curPriority == maxPriority)
				{
					topMoves.push_back(curMove);
				}
			}
		}
		++ind;
	}

	cout << "AI test 1" << endl;

	if (maxPriority <= 0)
	{
		cerr << "error, called getMove() after stalemate!";
		return Move(make_pair(0, 0), make_pair(0, 0), 0);
	}

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (seed);

	uniform_int_distribution<int> distribution(0, topMoves.size()-1);

	cout << "AI test 2" << endl;

	Move moveChoice = topMoves[distribution(generator)];

	this->removePiece(moveChoice.getOrigin());

	myPieces.push_back(moveChoice.getDest());

	return moveChoice;
}

Ai::Ai(int lvl, bool colour, vector< pair<int, int> > myPieces): lvl{lvl}, myPieces{myPieces}, Player{colour}
{
	
}

void Ai::removePiece(pair<int, int> toRemove)
{
	for (int i = 0; i < myPieces.size(); ++i)
	{
		if (myPieces[i] == toRemove)
		{
			myPieces.erase(myPieces.begin()+i);
		}
	}
}
