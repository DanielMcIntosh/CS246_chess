#ifndef _AI_
#define _AI_

#include <vector>
#include <utility>

#include "Player.h"

class Game;

class Ai: public Player{
private:
	std::vector< std::pair<int, int> > myPieces;
	int lvl;
public:
	Ai(int lvl, std::vector< std::pair<int, int> > myPieces);
	Move getMove(Game *g);
};

#endif