#include <vector>
#include <utility>

#include "Game.h"
#include "Move.h"
#include "Piece.h"

using namespace std;

const char defaultBoard[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
                                {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}, 
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                                {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};

Game::Game() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
        	//inverts coordinates to get [x][y]
            board[j][i] = Piece::constructPiece(defaultBoard[i][j]);
        }
    }
}

#define state_resign 2
#define state_mate 3
#define state_stale 1
#define state_invalid 0
#define state_normal -1
#define state_check -2

bool Game::doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p)
{
	cout << "1" << endl;
	if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0)
		return false;
	pair<int, int> diff = make_pair(x2-x1, y2-y1);
	cout << "1.5" << endl;
	cout << "origin = " << x1 << ", " << y1 << endl;
	cout << "dest = " << x2 << ", " << y2 << endl;
	cout << p->getChar() << endl;
	if ((!board[x2][y2] && !(p->isValidMove(diff))) || (board[x2][y2] && !(p->isValidCapture(diff))))
		return false;
	cout << "2" << endl;

	vector< pair<int, int> > moveReq = p->getMoveReq(diff);
	for (auto n: moveReq)
	{
		if (board[x1+n.first][y1+n.second])
			return false;
	}

	cout << "3" << endl;
	//check if the king is moving into check
	if ((p->getChar() | ('a' - 'A')) == 'k')
	{
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				Piece *enemy = board[x][y];
				if (enemy && !enemy->isValidCapture(make_pair(x2-x, y2-y)))
					return false;
				vector< pair<int, int> > enemyMoveReq = enemy->getMoveReq(make_pair(x2-x, y2-y));
				for (auto n: enemyMoveReq)
				{
					if (board[x+n.first][x+n.second])
						break;
				}
			}
		}
	}
	cout << "4" << endl;
	return true;
}

int Game::tryMove(Move &attempt, int priorityMask)
{
	int x1 = attempt.getOrigin().first;
	int y1 = attempt.getOrigin().second;
	int x2 = attempt.getDest().first;
	int y2 = attempt.getDest().second;
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

	if (board[x2][y2] && board[x2][y2]->getColour() != p->getColour())
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
	pair<int,int> wkThreat = isThreatened(wk_pos,false, true);
	if (wkThreat.first >= 0 && wkThreat.second >= 0){
		return false;
	}
	pair<int,int> bkThreat = isThreatened(bk_pos,true, true);
	if (bkThreat.first >= 0 && bkThreat.second >= 0){
		return false;
	}
	return true;
}


ostream& operator<<(ostream& os, const Game& gm) {
    for(int y = 7; y >= 0; --y) {
		os << (y+1) << ' ';
		for(int x = 0; x < 8; x++) {
			if(gm.board[x][y] == nullptr) {
				if(y % 2 == x % 2) {
					os << ' ';
				} else {
					os << '-';
				}
			} else {
				os << gm.board[x][y]->getChar();
			}
		}
		os << endl;
	}
	os << endl;
	os << "  adbcdefh" << endl;
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
pair<int,int> Game::isThreatened(pair<int,int> co, bool colour, bool checkingCapture){
	cout << "test a0" << endl;
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
	cout << "test a1" << endl;
	char dest;
	for(int dx = -1; dx <= 1 ; ++dx){ // covers Rook, Queen, Bishop, Enemy King capture vulnerability.
		for (int dy = -1; dy <= 1; ++dy){
			if (dx == 0 && dy == 0){
				continue;
			}
			for(int a = i+dx, b = j+dy; a >= 0 && a < 8 && b >= 0 && b < 8; a+=dx, b+=dy){
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

	cout << "test a2" << endl;

	int pawnXDiff[2][2] = {{1, -1}, {0, 0}};
	int index = checkingCapture ? 0 : 1;
	for (int i = 0; i < 2; ++i)
	{
		if (i+pawnXDiff[index][i] < 8 && board[i+pawnXDiff[index][i]][j+pawnDir]){ // Covers pawn capture vulnerability
			dest = board[i+pawnXDiff[index][i]][j+pawnDir]->getChar();
			if(dest == 'p'-diff){
				return make_pair(i+pawnXDiff[index][i],j+pawnDir);
			}
		}		
	}

	cout << "test a3" << endl;

	cout << "test a4" << endl;
	cout << "testing whether square " << i << ", " << j << "is threatened" << endl;

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
		int a = knightVuln[i].first;
		int b = knightVuln[i].second;
		if (a >= 0 && a < 8 && b >= 0 && b < 8){
			if (!board[a][b]){
				continue;
			}
			dest = board[a][b]->getChar();
			if ( dest == 'n'-diff){
				return make_pair(a,b);
			}
		}
		cout << "test a4." << i << endl;
	}
	cout << "test a5" << endl;

	return make_pair(-1,-1);
}





int Game::executeMove(Move &m){
	pair<int,int> origin = m.getOrigin();
	pair<int,int> dest = m.getDest();
	if(origin.first == dest.first && origin.second == dest.second){
		return state_resign;
	}
	Piece * p = board[origin.first][origin.second];
	if (!p)
	{
		cout << "p is null!" << endl;
	}
	cout << "test" << endl;
	cout << p->getChar() << endl;
	cout << "test2" << endl;
	if (!doesBoardPermit(origin.first,origin.second,dest.first,dest.second,p)){
		cout << "invalid 1" << endl;
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
					++kingsFound;
				}
			}
		}
	}

	//exposing self to check
	cout << "test a" << endl;
	pair<int,int> ourKingThreat = isThreatened(kingCords[curColour? 1:0],curColour, true);
	cout << "test b" << endl;
	if (ourKingThreat.first >= 0 || ourKingThreat.second >= 0){
			board[origin.first][origin.second] = board[dest.first][dest.second]; 
			board[dest.first][dest.second] = temp;
			cout << "invalid 2" << endl;
			return state_invalid;
	}
	delete temp;

	// check and check mate verification
	pair<int, int> eKingCoords = kingCords[curColour? 0:1];
	pair<int,int> enemyKingThreat =isThreatened(eKingCoords, !curColour, true);
	if(enemyKingThreat.first >= 0 || enemyKingThreat.second >= 0){
		Piece * threatPiece = board[enemyKingThreat.first][enemyKingThreat.second];
		for(int dx =-1; dx <= 1; ++dx){
			for(int dy = -1; dy <= 1; ++dy){
				if((dx == 0 && dy ==0) || !Game::isInBounds(eKingCoords.first + dx, eKingCoords.second + dy)){
					continue;
				}
				if(!board[eKingCoords.first + dx][eKingCoords.second + dy] || board[eKingCoords.first + dx][eKingCoords.second + dy]->getColour() == curColour){
					cout << "should only print this once" << endl;
					pair<int,int> result = isThreatened(make_pair(eKingCoords.first+dx,eKingCoords.second+dy),!curColour, true);
					if(result.first < 0 || result.second < 0){
						cout << "check 1" << endl;
						return state_check;
					}
				}
			}
		}
		vector< pair<int,int>> moveReq = threatPiece->getMoveReq(make_pair(eKingCoords.first-enemyKingThreat.first,eKingCoords.second-enemyKingThreat.second));
		for(auto n: moveReq){
			n.first += enemyKingThreat.first;
			n.second += enemyKingThreat.second;
			pair<int,int> result = isThreatened(n,curColour, false);
			if(result.first >= 0 || result.second >= 0){
				cout << "check 2" << endl;
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
					if(!board[x][x2]){
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
    
Game::~Game() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(this->board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }
}

bool Game::isInBounds(pair<int, int> co)
{
	return isInBounds(co.first, co.second);
}

bool Game::isInBounds(int x, int y)
{
	return (x < 8 && x >= 0 && y < 8 && y >= 0);
}