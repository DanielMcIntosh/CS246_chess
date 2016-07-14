#ifndef _GAME_
#define _GAME_

class Move;

class Game {
private:
	Piece *board[8][8] = {0};
	bool isValidMove(int x1, int y1, int x2, int y2, Piece *p);
public:
	int tryMove(Move &attempt);
};

#endif