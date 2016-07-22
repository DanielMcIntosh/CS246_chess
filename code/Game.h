#ifndef _GAME_
#define _GAME_

#include <vector>
#include <utility>
#include <iostream>

class Move;
class Piece;

class Game {
private:
	Piece *board[8][8];
	int startPlayer;
	bool doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p);

public:
	bool causeCheck(int priority);
	bool doesCapture(int priority);
	bool isSafe(int priority);
    Game();
	~Game();
	int tryMove(Move &attempt, int priorityMask);
	void setup(std::vector< std::pair<int, int> > playerPieces);
	bool isValidBoard();
	std::pair<int,int> isThreatened(std::pair<int,int> co, bool colour, bool checkingCapture, bool ignoreKing);
	int executeMove(Move &m);
	int getStartPlayer();
	friend std::ostream& operator<<(std::ostream& os, const Game& gm);
	static bool isInBounds(std::pair<int, int> co);
	static bool isInBounds(int x, int y);
};

#endif