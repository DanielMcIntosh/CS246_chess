#include <vector>
#include <utility>
#include <fstream>
#include "Game.h"
#include "Move.h"
#include "Piece.h"

#define inDebug

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

bool Game::isValidCastle(int x1, int y1, int x2, int y2, Piece* p)
{
	if (abs(x2-x1) != 2 || y1 != y2)
		return false;

	#ifdef inDebug
	cout << "castle relative move coordinates passed" << endl;
	#endif

	int rookX = x2 > x1 ? 7 : 0;

	#ifdef inDebug
	cout << "expecting rook at " << rookX << ", " << y1 << endl;
	#endif

	if (board[rookX][y1]->getChar() != (p->getColour() ? 'r' : 'R'))
		return false;
	
	#ifdef inDebug
	cout << "rook presence confirmed" << endl;
	#endif

	return (board[rookX][y1]->isFirstMove() && p->isFirstMove());
}

bool Game::doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p)
{
	#ifdef inDebug
	cout << "test doesBoardPermit 0" << endl;
	#endif

	if (!Game::isInBounds(x2, y2))
		return false;
	pair<int, int> diff = make_pair(x2-x1, y2-y1);

	#ifdef inDebug
	cout << "test doesBoardPermit 0.3" << endl;
	cout << "piece = " << p->getChar() << endl;
	cout << "origin = " << x1 << ", " << y1 << endl;
	cout << "destination = " << x2 << ", " << y2 << endl;
	#endif

	if (board[x2][y2] && board[x2][y2]->getColour() == p->getColour())
	{
		return false;
	}

	#ifdef inDebug
	cout << "test doesBoardPermit 0.6" << endl;
	#endif

	if ((p->getChar() | ('a'-'A')) == 'k') {
		if (!isValidCastle(x1, y1, x2, y2, p) && (abs(x1-x2) == 2)){
			return false;
		}
	} else if ((!board[x2][y2] && !(p->isValidMove(diff))) || (board[x2][y2] && !(p->isValidCapture(diff)))){
			
			//cout <<"This language sucks 2" << endl;
			return false;
	}


	#ifdef inDebug
	cout << "test doesBoardPermit 1" << endl;
	cout << "moveReq = {";
	#endif

	vector< pair<int, int> > moveReq = p->getMoveReq(diff);
	for (auto n: moveReq)
	{
		#ifdef inDebug
		cout << "<" << n.first << ", " << n.second << ">, ";
		#endif
		if(board[x1+n.first][y1+n.second] && board[x1+n.first][y1+n.second] != p
           && (p->getChar() != 'p' || p->getChar() != 'P'))
			return false;
	}

	#ifdef inDebug
	cout << "}" << endl;
	cout << "test doesBoardPermit 2" << endl;
	#endif


	//check if the king is moving into check
	if ((p->getChar() | ('a' - 'A')) == 'k')
	{
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				Piece *enemy = board[x][y];
				if (!enemy || enemy->getColour() == p->getColour())
					continue;
				if (!enemy->isValidCapture(make_pair(x2-x, y2-y)))
					continue;
				vector< pair<int, int> > enemyMoveReq = enemy->getMoveReq(make_pair(x2-x, y2-y));
				for (auto n: enemyMoveReq)
				{
					if (board[x+n.first][x+n.second])
						break;
				}
			}
		}
	}

	#ifdef inDebug
	cout << "test doesBoardPermit 3" << endl;
	#endif

	return true;
}

int Game::tryMove(Move &attempt, int priorityMask)
{
	#ifdef inDebug
	cout << "test tryMove 0" << endl;
	#endif

	int x1 = attempt.getOrigin().first;
	int y1 = attempt.getOrigin().second;
	int x2 = attempt.getDest().first;
	int y2 = attempt.getDest().second;
	if (!Game::isInBounds(x2, y2))
		return 0;
	if (board[x1][y1]->getColour() != attempt.getColour())
		return 0;
	Piece *p = board[x1][y1];
	if (!doesBoardPermit(x1, y1, x2, y2, p))
		return 0;

	#ifdef inDebug
	cout << "test tryMove 1" << endl;
	#endif

	int movePriority = 0b1001; //2^0 = valid, 2^1 = checking move, 2^2 = capturing move, 2^3 = avoids capture

	//check the board to see if this move puts opponent in check, or if it moves the piece into a threatened location
	bool kingFound = !causeCheck(priorityMask);

	#ifdef inDebug
	cout << "test tryMove 2" << endl;
	#endif

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

	#ifdef inDebug
	cout << "test tryMove 3" << endl;
	#endif

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
		for (int j = 0; j < 8; ++j){
			if (!board[i][j]){
				continue;
			}
			char c = board[i][j]->getChar();
			if (c == 'K'){
				++wking;
				wk_pos.first = i;
				wk_pos.second = j;
			} else if (c == 'k'){
				++bking;
				bk_pos.first =i;
				bk_pos.second = j;
			}
		}
	}
	if (!(bking == 1 && wking == 1)){
		return false;
	}
	// Check if there doesn't exist any Pawns on the first and last row.
	for (int j = 0; j < 8; ++j){
		char a = 0;
		char b = 0;
		if (board[j][7]){
		 a = board[j][7]->getChar();
		}
		if (board[j][0]){
		 b = board[j][0]->getChar();
		}
		if(a=='P'|| a== 'p' || b == 'P'||b== 'p'){
			return false;
		}
	}
	// Check for check mate situations (Vulnerability to Queen, Bishop and Rook).
	pair<int,int> wkThreat = isThreatened(wk_pos,false, true, false);
	if (wkThreat.first >= 0 && wkThreat.second >= 0){
		return false;
	}
	pair<int,int> bkThreat = isThreatened(bk_pos,true, true, false);
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
	os << "  abcdefgh" << endl;
    return os;
}

void Game::setup(vector< pair<int, int> > playerPieces[])
{
	char c;
	//for(int i = 0; i < 8; ++i){
	//	for(int j = 0; j < 8; ++j){
	//		if(board[i][j]){
	//			delete board[i][j];
	//			board[i][j] = nullptr;
	//		}
	//	}
	//}

	while (true){
		string temp;
		cin >> temp;
		c = temp[0];
		string s;
		if (c == '=')
		{
			cin >> s;
			//convert to lower case
			for (int i = 0; i < s.length(); ++i)
			{
				s[i] |= 'a'-'A';
			}
			startPlayer = (s == "black") ? 1 : 0;
			continue;
		}
		else if (temp == "load"){
			cout << "Please type file name: ";
			cin >> s;
			ifstream f (s);
			cout << "File read succesful" << endl;
			for(int j = 7; j >= 0 ; --j){
				for(int i = 0; i < 8 ; ++i){
					pair<int, int> coords (i,j);
					char c;
					f >> c;
					if (c == '_'){
						if (board[i][j]){
							bool pieceColour = board[i][j]->getChar() & ('a' - 'A');
							delete board[coords.first][coords.second];
							board[coords.first][coords.second] = nullptr;

							vector< pair<int, int> > pieces = playerPieces[pieceColour ? 1 : 0];
							for (int i = 0; i < pieces.size(); ++i)
							{
								if (pieces[i] == coords){
									playerPieces[pieceColour ? 1 : 0].erase(playerPieces[pieceColour ? 1 : 0].begin()+i);
								}
							}
						}
						continue;
					} else {
						if (board[i][j]){
							bool pieceColour = board[coords.first][coords.second]->getChar() & ('a' - 'A');
							delete board[coords.first][coords.second];
							board[coords.first][coords.second] = nullptr;

							vector< pair<int, int> > pieces = playerPieces[pieceColour ? 1 : 0];
							for (int i = 0; i < pieces.size(); ++i)
							{
								if (pieces[i] == coords){
									playerPieces[pieceColour ? 1 : 0].erase(playerPieces[pieceColour ? 1 : 0].begin()+i);
								}
							}
						}
						Piece * p = Piece::constructPiece(c);
						board[i][j] = p;
						playerPieces[(c & ('a' - 'A')) ? 1 : 0].push_back(coords);
					}
				}
			}
			string colour = "N";
			f >> colour;
			if (colour != "N"){
				if (colour == "W"){
					startPlayer = 0;
				} else {
					startPlayer = 1;
				}
			}
			continue;
		}
		else if (c == '-')
		{
			cin >> s;
			pair<int, int> coords = Piece::convertCoords(s);
			bool pieceColour = board[coords.first][coords.second]->getChar() & ('a' - 'A');
			delete board[coords.first][coords.second];
			board[coords.first][coords.second] = nullptr;

			vector< pair<int, int> > pieces = playerPieces[pieceColour ? 1 : 0];
			for (int i = 0; i < pieces.size(); ++i){
				if (pieces[i] == coords){
					playerPieces[pieceColour ? 1 : 0].erase(playerPieces[pieceColour ? 1 : 0].begin()+i);
				}
			}
			continue;
		}
		else if (c == '+')
		{	
			cin >> s;
			char pieceChar = s[0];
			Piece *p = Piece::constructPiece(pieceChar);
			string s2;
			cin >> s2;
			pair<int, int> coords = Piece::convertCoords(s2);

			if (board[coords.first][coords.second])
			{
				bool originalColour = board[coords.first][coords.second]->getChar() & ('a' - 'A');	
				vector< pair<int, int> > pieces = playerPieces[originalColour ? 1 : 0];
				for (int i = 0; i < pieces.size(); ++i)
				{
					if (pieces[i] == coords){
						playerPieces[originalColour ? 1 : 0].erase(playerPieces[originalColour ? 1 : 0].begin()+i);
						break;
					}
				}
			}

			delete board[coords.first][coords.second];
			board[coords.first][coords.second] = p;
			playerPieces[(pieceChar & ('a' - 'A')) ? 1 : 0].push_back(coords);
			continue;
		}
		else if (temp == "done"){
			bool result = this->isValidBoard();
			//cout<< "Reached 2" << endl;
			if(result){
				cout << "Setup is valid and completed!" << endl;
				cout << startPlayer<< endl;
				break;
			} else {
				cout << "Setup is invalid, please check again" << endl;
				continue;
			}
		}
		
	}
}

int Game::getStartPlayer()
{
	return startPlayer;
}

//checks if a (theoretical) piece at co from the player designated by colour would be under threat
pair<int,int> Game::isThreatened(pair<int,int> co, bool colour, bool checkingCapture, bool ignoreKing){
	int x = co.first;
	int y = co.second;

	#ifdef inDebug
	cout << "test a0" << endl;
	cout << "testing whether square " << x << ", " << y << " is threatened" << endl;
	#endif

	int diff = 0;
	int pawnDir;
	if (!colour){
		diff = 0;
		pawnDir = 1;
	} else {
		diff = 'a' - 'A';
		pawnDir = -1;
	}

	#ifdef inDebug
	cout << "test a1" << endl;
	#endif

	char dest;
	for(int dx = -1; dx <= 1 ; ++dx){ // covers Rook, Queen, Bishop, Enemy King capture vulnerability.
		for (int dy = -1; dy <= 1; ++dy){
			if ((dx == 0 && dy == 0) || !Game::isInBounds(x+dx, y+dy)){
				continue;
			}
			//check king capture vulnerability
			if (!ignoreKing && board[x+dx][y+dy] && board[x+dx][y+dy]->getChar() == 'k'-diff)
			{
				return make_pair(x+dx,y+dy);
			}
			for(int a = x+dx, b = y+dy; Game::isInBounds(a, b); a+=dx, b+=dy){
				if (!board[a][b]){
					continue;
				}
				dest = board[a][b]->getChar();
				if (dest == 'q'-diff){ 
					return make_pair(a,b);
				}
				if(abs(dx)==1 && abs(dy)==1 && dest == 'b'-diff){
					return make_pair(a,b);
				}
				if ((!dx || !dy) && dest == 'r'-diff){
					return make_pair(a,b);
				}
				break;
			}
		}
	}

	#ifdef inDebug
	cout << "test a2" << endl;
	#endif

	int pawnXDiff[2][2] = {{1, -1}, {0, 0}};
	int index = checkingCapture ? 0 : 1;
	for (int i = 0; i < 2; ++i)
	{
		if (x+pawnXDiff[index][i] < 8 && board[x+pawnXDiff[index][i]][y+pawnDir]){ // Covers pawn capture vulnerability
			dest = board[x+pawnXDiff[index][i]][y+pawnDir]->getChar();
			if(dest == 'p'-diff){
				return make_pair(x+pawnXDiff[index][i],y+pawnDir);
			}
		}		
	}

	#ifdef inDebug
	cout << "test a3" << endl;
	#endif

	pair<int,int> knightVuln [8]; // Covers Knight capture vulnerability
	knightVuln[0] = make_pair(x+2,y+1);
	knightVuln[1] = make_pair(x+2,y-1);
	knightVuln[2] = make_pair(x-2,y+1);
	knightVuln[3] = make_pair(x-2,y-1);
	knightVuln[4] = make_pair(x+1,y+2);
	knightVuln[5] = make_pair(x+1,y-2);
	knightVuln[6] = make_pair(x-1,y+2);
	knightVuln[7] = make_pair(x-1,y-2);
	for(int i = 0; i < 8; ++i){
		int a = knightVuln[i].first;
		int b = knightVuln[i].second;
		if (!Game::isInBounds(a, b)) {
			continue;
		}
		if (!board[a][b]){
			continue;
		}
		dest = board[a][b]->getChar();
		if ( dest == 'n'-diff){
			return make_pair(a,b);
		}

		#ifdef inDebug
		cout << "test a4." << i << endl;
		#endif
	}

	#ifdef inDebug
	cout << "test a5" << endl;
	#endif

	return make_pair(-1,-1);
}





int Game::executeMove(Move &m){
	pair<int,int> origin = m.getOrigin();
	pair<int,int> dest = m.getDest();
	if(origin.first == dest.first && origin.second == dest.second){
		return state_resign;
	}
	Piece *p = board[origin.first][origin.second];

	if (!p)
	{
		#ifdef inDebug
		cout << "invalid 0: no piece found" << endl;
		#endif

		return state_invalid;
	}

	if (p->getColour() != m.getColour())
	{
		#ifdef inDebug
		cout << "invalid 0.5: wrong colour" << endl;
		#endif
		return state_invalid;
	}

	if (!doesBoardPermit(origin.first,origin.second,dest.first,dest.second,p)){
		#ifdef inDebug
		cout << "invalid 1" << endl;
		#endif
		return state_invalid;
	}
	bool curColour = board[origin.first][origin.second]->getColour();
	
	Piece *temp = board[dest.first][dest.second];
	board[dest.first][dest.second] = p;
	board[origin.first][origin.second] = nullptr;

	if ((board[dest.first][dest.second]->getChar() | ('a' - 'A')) == 'k' && abs(dest.first - origin.first) == 2)
	{
		int rookX = dest.first - origin.first > 0 ? 7 : 0;
		board[origin.first + (dest.first-origin.first)/2][origin.second] = board[rookX][origin.second];
		board[rookX][origin.second] = nullptr;
	}

	pair <int,int> kingCords [2];
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
	#ifdef inDebug
	cout << "test a" << endl;
	#endif

	pair<int,int> ourKingThreat = isThreatened(kingCords[curColour? 1:0], curColour, true, false);

	#ifdef inDebug
	cout << "test b" << endl;
	#endif

	if (ourKingThreat.first >= 0 || ourKingThreat.second >= 0){
			if ((board[dest.first][dest.second]->getChar() | ('a' - 'A')) == 'k' && abs(dest.first - origin.first) == 2)
			{
				int rookX = dest.first - origin.first > 0 ? 7 : 0;
				board[rookX][origin.second] = board[origin.first + (dest.first-origin.first)/2][origin.second];
				board[origin.first + (dest.first-origin.first)/2][origin.second] = nullptr;
			}
			board[origin.first][origin.second] = board[dest.first][dest.second]; 
			board[dest.first][dest.second] = temp;
			
			#ifdef inDebug
			cout << "invalid 2: kings is threatened by piece at " << ourKingThreat.first << ", " << ourKingThreat.second << endl;
			#endif

			return state_invalid;
	}

	//move is known to be valid, proceed with making changes to board permanent
	board[dest.first][dest.second]->setFirstMove();
	if ((board[dest.first][dest.second]->getChar() | ('a' - 'A')) == 'k' && abs(dest.first - origin.first) == 2)
	{
		board[origin.first + (dest.first-origin.first)/2][origin.second]->setFirstMove();
	}
	delete temp;

	// check and check mate verification
	pair<int, int> eKingCoords = kingCords[curColour? 0:1];
	pair<int,int> enemyKingThreat =isThreatened(eKingCoords, !curColour, true, false);
	if(enemyKingThreat.first >= 0 || enemyKingThreat.second >= 0){
		Piece * threatPiece = board[enemyKingThreat.first][enemyKingThreat.second];
		for(int dx =-1; dx <= 1; ++dx){
			for(int dy = -1; dy <= 1; ++dy){
				if((dx == 0 && dy ==0) || !Game::isInBounds(eKingCoords.first + dx, eKingCoords.second + dy)){
					continue;
				}
				if(!board[eKingCoords.first + dx][eKingCoords.second + dy] || board[eKingCoords.first + dx][eKingCoords.second + dy]->getColour() == curColour){
					pair<int,int> result = isThreatened(make_pair(eKingCoords.first+dx,eKingCoords.second+dy),!curColour, true, false);
					if(result.first < 0 || result.second < 0){
						
						#ifdef inDebug
						cout << "check 1: king not threatened on all sides" << endl;
						#endif

						return state_check;
					}
				}
			}
		}
		vector< pair<int,int>> moveReq = threatPiece->getMoveReq(make_pair(eKingCoords.first-enemyKingThreat.first,eKingCoords.second-enemyKingThreat.second));
		for(auto n: moveReq){
			n.first += enemyKingThreat.first;
			n.second += enemyKingThreat.second;
			pair<int,int> result = isThreatened(n,curColour, false, true);
			if(result.first >= 0 || result.second >= 0){
				
				#ifdef inDebug
				cout << "check 2: player can block threat" << endl;
				#endif

				return state_check;
			}
		}

		pair<int,int> threat = isThreatened(enemyKingThreat,curColour, false, true);
		if(threat.first >= 0 || threat.second >= 0){
			
			#ifdef inDebug
			cout << "check 3: player can capture threat" << endl;
			#endif

			return state_check;
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