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
	int tryMove(Move &attempt);
	void setup();
	bool isValidBoard();
	pair<int,int> isThreatened(pair<int,int> co, bool colour);
	int executeMove(Move &m);
	int getStartPlayer();
	friend ostream& Game::operator<<(ostream& os, const Game& gm);
};

#endif