#include <vector>
#include <utilities>

#include "Game.h"
#include "Move.h"
#include "Piece.h"
using namespace std;

bool Game::isValidMove(int x1, int y1, int x2, int y2, Piece *p)
{
	if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0)
		return false;
	pair<int, int> diff = make_pair(x2-x1, y2-y1);
	if (!p->isValid(diff))
		return false;

	vector< pair<int, int> > moveReq = p->getMoveReq(diff);
	for (pair<int, int> *i = moveReq.begin(); i != moveReq.end(); ++i)
	{
		if (board[x1+i->first][y1+i->second])
			return false;
	}
	return true;
}

int Game::tryMove(Move &attempt)
{
	int x1 = attempt.getOrigin().first, y1 = attempt.getOrigin().second;
	int x2 = attempt.getDestination().first, y2 = attempt.getDestination().second;
	if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0)
		return 0;
	Piece *p = board[x1][y1];
	if (!isValidMove(x1, y1, x2, y2, p))
		return 0;


	int movePriority = 0b1001; //2^0 = valid, 2^1 = checking move, 2^2 = capturing move, 2^3 = avoids capture

	for (int x = 0, bool kingFound = false, bool unsafe = false; !(kingFound && unsafe) && x < 8; ++x)
	{
		for (int y = 0; !(kingFound && unsafe) && y < 8; ++y)
		{
			if (!kingFound && isValidMove(x2, y2, x, y, p) && board[x][y].getChar() == 'K' + p->getColour() ? 0 : ('a' - 'A'))
			{
				movePriority |= 0b0010;
				kingFound = true;
			}
			if (!unsafe && isValidMove(x, y, x2, y2, board[x][y]))
			{
				movePriority &= 0b0111;
				unsafe = true;
			}
		}
	}

	if (board[x2][y2] && board[x2][y2]->getColour() != p.getColour())
	{
		movePriority |= 0b0100;
	}

	return movePriority;
}
