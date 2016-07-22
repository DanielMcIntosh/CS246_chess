#ifndef _PLAYER_
#define _PLAYER_

class Move;
class Game;

class Player {
public:
	virtual Move getMove(Game *g) =0; 
	virtual ~Player();
};

#endif
