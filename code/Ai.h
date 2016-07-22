#ifndef _AI_
#define _AI_

#include <vector>
#include <utility>

#include "Player.h"

class Game;

class Ai: public Player{
private:
	std::vector< std::pair<int, int> > myPieces;
public:
	Ai(int lvl);
	Move getMove(Game *g);
};

#endif