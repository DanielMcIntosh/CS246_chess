#ifndef _PLAYER_
#define _PLAYER_

#include <utility>

class Move;
class Game;

class Player {
public:
	virtual Move getMove(Game *g) =0; 
	virtual ~Player();
	virtual void removePiece(std::pair<int, int> toRemove) =0;
};

#endif
