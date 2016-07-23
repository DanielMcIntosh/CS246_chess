#ifndef _HUMAN_
#define _HUMAN_

#include "Player.h"

class Game;

class Human: public Player{
public:
	Human(bool colour);
	Move getMove(Game *g) override;
	void removePiece(std::pair<int, int> toRemove);
};

#endif