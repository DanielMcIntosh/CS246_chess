#ifndef _HUMAN_
#define _HUMAN_

#include "Player.h"

class Game;

class Human: public Player{
public:
	Human();
	Move getMove(Game *g) override;
};

#endif