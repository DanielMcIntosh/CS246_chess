#ifndef _GAME_
#define _GAME_

#include <vector>
#include <utility>
#include <iostream>

#include "View.h"

class Move;
class Piece;

class Game {
private:
	Piece *board[8][8];
	int startPlayer;
	std::pair<int, int> ghostPawn = std::make_pair(-1, -1);
	bool doesBoardPermit(int x1, int y1, int x2, int y2, Piece *p);
	bool isValidCastle(int x1, int y1, int x2, int y2, Piece *p);
public:
	char getPosChar(int x, int y);
	bool causeCheck(int priority);
	bool doesCapture(int priority);
	bool isSafe(int priority);
    Game();
	~Game();
	int tryMove(Move &attempt, int priorityMask);
	void setup(std::vector< std::pair<int, int> > playerPieces[], View v);
	bool isValidBoard();
	std::pair<int,int> isThreatened(std::pair<int,int> co, bool colour, bool checkingCapture, bool ignoreKing);
	int executeMove(Move &m);
	int getStartPlayer();
	friend std::ostream& operator<<(std::ostream& os, const Game& gm);
	
	static bool isInBounds(std::pair<int, int> co);
	static bool isInBounds(int x, int y);

	static char toLower(char c);
};

#endif