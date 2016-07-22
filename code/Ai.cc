#include "Ai.h"
#include "Move.h"

#include <vector>
#include <utility>

using namespace std;

const int priorityMasks[4] = {0b0001, 0b0111, 0b1111, 0b1111};

Move Ai::getMove(Game *g)
{
	vector<Move> topMoves{};
	int maxPriority = 0;
	for (auto i: this->myPieces)
	{
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				Move curMove = new Move(*i, make_pair(x, y), colour ? 'q' : 'Q');
				int curPriority = Game->tryMove(curMove, priorityMasks[this->lvl]);
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
	}

	return Move(make_pair(0, 0), make_pair(0, 0), 0);
}

Ai::Ai(int lvl, bool colour, vector< pair<int, int> > myPieces): lvl{lvl}, colour{colour}, myPieces{myPieces}
{
	
}
