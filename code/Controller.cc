#include <iostream>
#include <vector>
#include <utility>

#include "Controller.h"
#include "Player.h"
#include "Human.h"
#include "Ai.h"
#include "Game.h"
#include "Move.h"
#include "View.h"

#define numPlayers 2

using namespace std;

int main(){
	Game * curGame = new Game();

	View v = View();
	int score [numPlayers] = {0};
	vector< pair<int, int>> playerPieces[numPlayers]{};
	for (int i = 0; i < numPlayers; ++i)
	{
		int y = i ? 6 : 0;
		for (int x = 0; x < 8; ++x)
		{
			playerPieces[i].push_back(make_pair(x, y));
			playerPieces[i].push_back(make_pair(x, y+1));
		}
	}

	string input;

	v.welcome();

	while (cin >> input){
		if (input == "setup"){
			curGame->setup(playerPieces, v);
		} else if (input == "game"){
			Player *p[numPlayers];
			for (int i = 0 ; i < numPlayers; ++i){
				string arg;
				cin >> arg;
				if (arg == "human"){
					p[i] = new Human((bool)i);
				} else {
					int lvl = arg[9] - '0';
					p[i] = new Ai(lvl, (bool)i, playerPieces[i]);
				}
			}

			int winner = runGame(p, curGame, v);
			if (winner == -1){
				score[0] += 0.5;
				score[1] += 0.5;
			} else {
				++score[winner];
			}

			for (int i = 0; i < numPlayers; ++i)
			{
				delete p[i];
			}
			playerPieces[0].clear();
			playerPieces[1].clear();
			for (int i = 0; i < numPlayers; ++i)
			{
				int y = i ? 6 : 0;
				for (int x = 0; x < 8; ++x)
				{
					playerPieces[i].push_back(make_pair(x, y));
					playerPieces[i].push_back(make_pair(x, y+1));
				}
			}
			delete curGame;
			curGame = new Game();

		} else {
			cerr << "Invalid Command! Please enter setup or game." << endl;
		}
	}
	delete curGame;
	v.printScore(score[0], score[1]);
}


int runGame(Player *p[], Game *curGame, View view){
	cout << *curGame;
	int start = curGame->getStartPlayer();
	int result = 0;
	for (int moveCount = 0; !result ; ++moveCount){
		Move m = p[(moveCount + start)%numPlayers]->getMove(curGame);
		int moveResult = curGame->executeMove(m);
		if (!moveResult){
			--moveCount;
			cerr << "Invalid Move" << endl;
			continue;
		}

		p[(moveCount + start + 1)%numPlayers]->removePiece(m.getDest());

		result = reactToState(moveResult,(moveCount + start)%numPlayers, view);
		cout << *curGame;
	}
	if (result < 0)
		return -1;
	return result % 2;
}

//returns false when endgame state reached
int reactToState(int state, int curPlayer, View view){
	view.reactToTurn(state, curPlayer);
	if (state == 1){ //stalemate
		return -1;
	}
	return (state > 0) ? ((curPlayer + state) % 2 + 1) : 0;
}
