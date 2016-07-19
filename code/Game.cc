#include <vector>
#include <utilities>

#include "Game.h"
#include "Move.h"
#include "Piece.h"
using namespace std;

bool Game::doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p)
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

int Game::tryMove(Move &attempt, int priorityMask)
{
	int x1 = attempt.getOrigin().first, y1 = attempt.getOrigin().second;
	int x2 = attempt.getDestination().first, y2 = attempt.getDestination().second;
	if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0)
		return 0;
	Piece *p = board[x1][y1];
	if (!doesBoardPermit(x1, y1, x2, y2, p))
		return 0;

	int movePriority = 0b1001; //2^0 = valid, 2^1 = checking move, 2^2 = capturing move, 2^3 = avoids capture

	//check the board to see if this move puts opponent in check, or if it moves the piece into a threatened location
	bool kingFound = !causeCheck(priorityMask);
	for (int x = 0; x < 8 && (!kingFound || isSafe(movePriority&priorityMask)); ++x)
	{
		for (int y = 0; (!kingFound || isSafe(movePriority&priorityMask)) && y < 8; ++y)
		{
			if (!board[x][y])
				continue;
			//can p attack opposing king (at x, y) from x2, y2?
			if (!kingFound && (kingFound = (board[x][y].getChar() == p->getColour() ? 'K' : 'k')) && doesBoardPermit(x2, y2, x, y, p))
			{
				movePriority |= 0b0010;
			}
			//can p get captured by the piece at x, y
			if (isSafe(movePriority&priorityMask) && doesBoardPermit(x, y, x2, y2, board[x][y]))
			{
				movePriority &= ~0b1000;
			}
		}
	}

	if (board[x2][y2] && board[x2][y2]->getColour() != p.getColour())
	{
		movePriority |= 0b0100;
	}

	return movePriority;
}

bool Game::causeCheck(int priority)
{
	return priority & 0b10;
}

bool Game::doesCapture(int priority)
{
	return priority & 0b100;
}

bool Game::isSafe(int priority)
{
	return priority & 0b1000;
}


ostream& Game::operator<<(ostream& os, const Game& gm) {
    for(int i = 0; i < 8; i++) {
		os << (8 - i) << ' ';
		for(int j = 0; j < 8; j++) {
			if(gm.board[i][j] == nullptr) {
				if(i % 2 == 0 && j % 2 == 0) {
					os << ' ';
				} else if (i % 2 == 1 && j & 2 == 1) {
					os << ' ';
				} else {
					os << '-';
				}
			} else {
				os << gm.board[i][j].getChar();
			}
		}
		os << endl;
	}
	os << endl;
	os << "  adbcdefh";
    return os;
}