#ifndef _AI_
#define _AI_

#include <vector>

#include "Player.h"

class Ai: public Player{
private:
	vector< pair<int, int> > myPieces;
public:
	Ai(int lvl);
};

#endif