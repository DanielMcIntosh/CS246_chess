#include <vector>
#include <utilities>

#include "Game.h"
#include "Move.h"
#include "Piece.h"
using namespace std;

#define state_resign 2
#define state_mate 3
#define state_stale 1
#define state_invalid 0
#define state_normal -1
#define state_check -2


bool Game::doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p)
{
	if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0)
		return false;
	pair<int, int> diff = make_pair(x2-x1, y2-y1);
	if ((!board[x2][y2] && !p->isValidMove(diff)) || (board[x2][y2] && !p->isValidCapture(diff)))
		return false;

	vector< pair<int, int> > moveReq = p->getMoveReq(diff);
	for (pair<int, int> *i = moveReq.begin(); i != moveReq.end(); ++i)
	{
		if (board[x1+i->first][y1+i->second])
			return false;
	}

	//check if the king is moving into check
	if (p->getChar() | ('a' - 'A') == 'k')
	{
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				Piece *enemy = board[x][y];
				if (enemy && !enemy->isValidCapture(make_pair(x2-x, y2-y)))
					return continue;
				vector< pair<int, int> > enemyMoveReq = enemy->getMoveReq();
				for (pair<int, int> *i = enemyMoveReq.begin(); i != enemyMoveReq.end(); ++i)
				{
					if (board[x+i->first][x+i->second])
						break;
				}
			}
		}
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
			if (!kingFound && (kingFound = (board[x][y]->getChar() == p->getColour() ? 'K' : 'k')) && doesBoardPermit(x2, y2, x, y, p))
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
	pair<int,int> wkThreat == isThreatened(wk_pos,false);
	if (wkThreat.first >= 0 && wkThreat.second >= 0){
		return false;
	}
	pair<int,int> bkThreat == isThreatened(bk_pos,true);
	if (bkThreat.first >= 0 && bkThreat.second >= 0){
		return false;
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
				os << gm.board[i][j]->getChar();
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

//checks if a (theoretical) piece at co from the player designated by colour would be under threat
pair<int,int> Game::isThreatened(pair<int,int> co, bool colour){
	int i = co.first;
	int j = co.second;
	int diff = 0;
	int pawnDir;
	if (!colour){
		diff = 0;
		pawnDir = 1;
	} else {
		diff = 'a' - 'A';
		pawnDir = -1;
	}
	char dest;
	for(int dx = -1; dx <= 1 ; ++dx){ // covers Rook, Queen, Bishop, Enemy King capture vulnerability.
		for (int dy = -1; dy <= 1; ++dy){
			if (dx == 0 && dy == 0){
				continue;
			}
			for(int a = i+dx, b = j+dy; a >= 0 && a < 8 && b >= 0 && b < 8; a+dx, b+dy){
				if (a == i+dx && b == j+dy && board[a][b]){ // Check first place if its a king.
					dest = board[a][b]->getChar();
					if (dest == 'k'-diff){
						return make_pair(a,b);
				}
				break;
				}
				if(board[a][b]){
					dest = board[a][b]->getChar();
					if (dest == 'q'-diff){ 
						return make_pair(a,b); }
					if(abs(dx)==1 && abs(dy)==1 && dest == 'b'-diff){
						return make_pair(a,b);
					} else {
						if (dest == 'r'-diff){
							return make_pair(a,b);
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
			return make_pair(i+1,j+pawnDir);
		}
	}
	if (i-1 >= 0 && board[i-1][j+pawnDir]){
		dest = board[i-1][j+pawnDir]->getChar();
		if(dest == 'p'-diff){
			return make_pair(i-1,j+pawnDir);
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
				return make_pair(a,b);
			}
		}
	}
	return make_pair(-1,-1);
}





int Game::executeMove(Move &m){
	if(m.origin.first == m.destination.first && m.origin.second == m.destination.second){
		return state_resign;
	}
	pair<int,int> origin = m.getOrigin();
	pair<int,int> dest = m.getDest();
	Piece * p = board[origin.first][origin.second];
	if (!doesBoardPermit(origin.first,origin.second,dest.first,dest.second,p)){
		return state_invalid;
	}
	bool curColour = board[origin.first][origin.second]->getColour();
	pair <int,int> kingCords [2];
	Piece * temp = board[dest.first][dest.second];
	board[dest.first][dest.second] = p;
	board[origin.first][origin.second] = nullptr;
	
	// search for kings positions
	for(int x = 0, kingsFound = 0; kingsFound < 2 && x < 8; ++x){
		for(int y = 0; y < 8 && kingsFound < 2; ++y){
			if(board[x][y]){
				char c = board[x][y]->getChar();
				if (c == 'K'){
					kingCords[0].first = x;
					kingCords[0].second = y;
					++kingsFound;
				} else if (c == 'k'){
					kingCords[1].first = x;
					kingCords[1].second = y;
					++kingFound;
				}
			}
		}
	}

	//exposing self to check
	pair<int,int> ourKingThreat = isThreatened(kingCords[curColour? 1:0],curColour);
	if (ourKingThreat.first >= 0 || ourKingThreat.second >= 0){
			board[origin.first][origin.second] = board[dest.first][dest.second]; 
			board[dest.first][dest.second] = temp;
			return state_invalid;
	}
	delete temp;

	// check and check mate verification
	eKingCoords = kingCords[curColour? 0:1];
	pair<int,int> enemyKingThreat =isThreatened(eKingCords, !curColour);
	if(enemyKingThreat.first >= 0 || enemyKingThreat.second >= 0){
		Piece * threatPiece = board[enemyKingThreat.first][enemyKingThreat.second];
		for(int dx =-1; dx <= 1; ++dx){
			for(int dy = -1; dy <= 1; ++dy){
				if(dx == 0 && dy ==0){
					continue;
				}
				if(!board[eKingCords.first+dx][eKingCords+dy]){
					pair<int,int> result = isThreatened(make_pair(eKingCords.first+dx,eKingCords+dy),!curColour);
					if(result.first < 0 || result.second < 0){
						return state_check;
					}
				}
			}
		}
		vector< pair<int,int>> moveReq = threatPiece.getMoveReq(make_pair(eKingCords.first-enemyKingThreat.first,eKingCords.second-enemyKingThreat.second));
		for(auto n: moveReq){
			n.first += enemyKingThreat.first;
			n.second += enemyKingThreat.second;
			pair<int,int> result = isThreatened(n,curColour);
			if(result.first >= 0 || result.second >= 0){
				return state_check;
			}
		}
		return state_mate;
	}


	//stale check
	for(int x = 0; x < 8 ; ++x){
		for (int y = 0; y < 8; ++y){
			Piece * curPiece = board[x][y];
			if(!curPiece){
				continue;
			}
			for(int x2 = 0; x2 < 8 ; ++x2){
				for(int y2 = 0; y2 < 8 ; ++y2){
					if (x == x2 && y == y2){
						continue;
					}
					if(!board[x1][x2]){
						continue;
					}
					if (doesBoardPermit(x,y,x2,y2,curPiece)){
						return state_normal;
					}
				}
			}

		}
	}
	return state_stale;
}
