#ifndef _PLAYER_
#define _PLAYER_

class Move;
class Player {
public:
	virtual Move getMove() =0; 
};

#endif
