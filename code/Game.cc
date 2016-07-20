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
			char c = board[i][j]->getChar();
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
		char a = board[0][j]->getChar();
		char b = board[7][j]->getChar();
		if(a=='P'|| a== 'p' || b == 'P'||b== 'p'){
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

		for(int k = -1; k <= 1 ; ++k){ // covers Rook, Queen, Bishop, Enemy King capture vulnerability.
			for (int h = -1; h <= 1; ++h){
				if (k == 0 && h == 0){
					continue;
				}
				for(int a = i+k, int b = j+h; a >= 0 && a < 8 && b >= 0 && b < 8; a+k, b+h){
					if (a == i+k && b == j+h && board[a][b]){ // Check first place if its a king.
						dest = board[a][b].getChar();
						if (dest == 'k'-diff){
							return false;
						}
						break;
					}
					if(board[a][b]){
						dest = board[a][b].getChar();
						if (dest == 'q'-diff){ return false; }
						if(abs(k)==1 && abs(h)==1 && dest == 'b'-diff){
							return false;
						} else {
							if (dest == 'r'-diff){
								return false;
							}
						}
						break;
					}
				}
			}
		}

		if (i+1 < 8 && board[i+1][j+pawnDir]){ // Covers pawn capture vulnerability
			dest = board[i+1][j+pawnDir]->getChar();
			if(dest == 'p'-diff){
				return false;
			}
		}
		if (i-1 >= 0 && board[i-1][j+pawnDir]){
			dest = board[i-1][j+pawnDir]->getChar();
			if(dest == 'p'-diff){
				return false;
			}
		}

		pair<int,int> knightVuln [8]; // Covers Knight capture vulnerability
		knightVuln[0] = make_pair(i+2,j+1);
		knightVuln[1] = make_pair(i+2,j-1);
		knightVuln[2] = make_pair(i-2,j+1);
		knightVuln[3] = make_pair(i-2,j-1);
		knightVuln[4] = make_pair(i+1,j+2);
		knightVuln[5] = make_pair(i+1,j-2);
		knightVuln[6] = make_pair(i-1,j+2);
		knightVuln[7] = make_pair(i-1,j-2);
		for(int i = 0; i < 8; ++i){
			if (knightVuln[i].first >= 0 && knightVuln[i].first < 8 && knightVuln[i].second >= 0 && knightVuln[i].second < 8){
				int a = knightVuln[i].first;
				int b = knightVuln[i].second;
				dest = board[a][b]->getChar();
				if ( dest == 'n'-diff){
					return false;
				}
			}
		}
	}
	return true;
}


ostream& Game::operator<<(ostream& os, const Game& gm) {
    for(int i = 0; i < 8; i++) {
		os << (8 - i) << ' ';
		for(int j = 0; j < 8; j++) {
			if(gm.board[i][j] == nullptr) {
				if(i % 2 == j % 2) {
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

void Game::setup()
{
	char c;
	do {
		string temp;
		cin >> temp;
		c = temp[0];
		string s;
		cin >> s;
		if (c == '=')
		{
			//convert to lower case
			for (int i = 0; i < s.length(); ++i)
			{
				s[i] |= 'a'-'A';
			}
			startPlayer = (s == "black") ? 1 : 0;
			continue;
		}
		else if (c == '-')
		{
			pair<int, int> coords = Piece::convertCoords(s);
			delete board[coords.first][coords.second];
			board[coords.first][coords.second] = nullptr;
			continue;
		}
		else if (c == '+')
		{
			char pieceChar = s[0];
			Piece *p = Piece::constructPiece(pieceChar);
			string s2;
			cin >> s2;
			pair<int, int> coords = Piece::convertCoords(s2);
			delete board[coords.first][coords.second];
			board[coords.first][coords.second] = p;
			continue;
		}
	} while (c != 'd');
}

int Game::getStartPlayer()
{
	return startPlayer;
}