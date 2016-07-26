#ifndef _CONTROLLER_
#define _CONTROLLER_

class Game;

class Player;
int runGame(Player *p[], Game *curGame);
int reactToState(int state, int curPlayer);

#endif
