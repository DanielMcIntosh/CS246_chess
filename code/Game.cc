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

bool Game::isValidBoard(){
	int bking = 0;
	int wking = 0;
	pair<int,int> bk_pos(0,0);
	pair<int,int> wk_pos(0,0);
	for(int i = 0; i < 8; ++i){  // Checking there only exist 1 king of each colour
		for (int j = 0; i < 8; ++j){
			char c = board[i][j].getChar();
			if (c == 'K'){
				++wking;
				wk_pos.first = i;
				wk_pos.second = j;
			} else if (c == 'k'){
				bk_pos.first =i;
				bk_pos.second = j;
			}
		}
	}
	if (bking != 1 && wking != 1){
		return false;
	}
	// Check if there doesn't exist any Pawns on the first and last row.
	for (int j = 0; j < 8; ++j){
		char a = board[0][j].getChar();
		char b = board[7][j].getChar();
		if(a=='P'||b== 'p'){
			return false;
		}
	}
	// Check for check mate situations (Vulnerability to Queen, Bishop and Rook).
	for(int x = 0; x < 2; ++x){
		int i;
		int j;
		int diff = 0;
		int pawnDir;
		if (x == 0){
			i = wk_pos.first;
			j = wk_post.second;
			diff = 0;
			pawnDir = 1;
		} else {
			i = bk_pos.first;
			j = bk_post.second;
			diff = 'a' - 'A';
			pawnDir = -1;
		}
		char dest;
		for(int k = i+1; k < 8 ; ++k){
			if (board[k][j]){
				dest = board[k][j].getChar();
				if (dest == 'q'-diff || dest == 'r'-diff){ return false; }
				break;
			}
		}
		for(int k = i-1; k >= 0 ; --k){
			if (board[k][j]){
				dest = board[k][j].getChar();
				if (dest == 'q'-diff || dest == 'r'-diff){ return false; }
				break;
			}
		}
		for(int k = j+1; k < 8 ; ++k){
			if (board[k][j]){
				dest = board[i][k].getChar();
				if (dest == 'q'-diff || dest == 'r'-diff){ return false; }
				break;
			}
		}
		for(int k = j-1; k >= 0 ; --k){
			if (board[i][k]){
				dest = board[k][j].getChar();
				if (dest == 'q'-diff || dest == 'r'-diff){ return false; }
				break;
			}
		}
		for(int k = i+1, int h = j+1; k < 8 && h < 8 ; ++k,++h){
			if (board[k][h]){
				dest = board[k][h].getChar();
				if (dest == 'q'-diff || dest == 'b'-diff){ return false; }
				break;
			}
		}
		for(int k = i-1, int h = j-1; k >= 0 && h >= 0 ; --k,--h){
			if (board[k][h]){
				dest = board[k][h].getChar();
				if (dest == 'q'-diff || dest == 'b'-diff){ return false; }
				break;
			}
		}
		for(int k = i+1, int h = j-1; k < 8 && h >= 0 ; ++k,--h){
			if (board[k][h]){
				dest = board[k][h].getChar();
				if (dest == 'q'-diff || dest == 'b'-diff){ return false; }
				break;
			}
		}
		for(int k = i-1, int h = j+1; k >= 0 && h < 8 ; --k,++h){
			if (board[k][h]){
				dest = board[k][h].getChar();
				if (dest == 'q'-diff || dest == 'b'-diff){ return false; }
				break;
			}
		}
		if (i+1 < 8 && board[i+1][j+pawnDir]){
			dest = board[i+1][j+pawnDir].getChar();
			if(dest == 'p'-diff){
				return false;
			}
		}
		if (i-1 >= 0 && board[i-1][j+pawnDir]){
			dest = board[i-1][j+pawnDir].getChar();
			if(dest == 'p'-diff){
				return false;
			}
		}	
		for (int i = 0; i < 8; ++i){
			
		}

	}
	return true;
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


