#ifndef _CONTROLLER_
#define _CONTROLLER_
#include "View.h"

class Game;

class Player;
int runGame(Player *p[], Game *curGame, View view);
int reactToState(int state, int curPlayer, View view);

#endif
