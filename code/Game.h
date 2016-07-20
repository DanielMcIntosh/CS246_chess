#ifndef _GAME_
#define _GAME_

class Move;
class Piece;

class Game {
private:
	Piece *board[8][8];
	int startPlayer;
	bool doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p);

	bool causeCheck(int priority);
	bool doesCapture(int priority);
	bool isSafe(int priority);
public:
    Game();
	int tryMove(Move &attempt);
	void setup();
	bool isValidBoard();
	int executeMove(Move &m);
	int getStartPlayer();
    ~Game();
};

#endif