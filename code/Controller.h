#ifndef _CONTROLLER_
#define _CONTROLLER_

class Game;

class Player;
void runGame(Player *p[], Game *curGame);
bool reactToState(int state, int curPlayer);

#endif
