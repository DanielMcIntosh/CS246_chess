#ifndef _AI_
#define _AI_

#include <vector>
#include <utility>

#include "Player.h"

class Game;

class Ai: public Player{
private:
	int lvl;
	std::vector< std::pair<int, int> > myPieces;
public:
	Ai(int lvl, bool colour, std::vector< std::pair<int, int> > myPieces);
	Move getMove(Game *g);
	void removePiece(std::pair<int, int> piece);
};

#endif