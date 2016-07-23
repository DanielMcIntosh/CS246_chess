#include <iostream>
#include <vector>
#include <utility>

#include "Controller.h"
#include "Player.h"
#include "Human.h"
#include "Ai.h"
#include "Game.h"
#include "Move.h"

#define numPlayers 2

using namespace std;

int main(){
	Game * curGame = new Game();
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
	while (cin>>input){
		if (input == "setup"){
			curGame->setup(playerPieces);
		} else if (input == "game"){
			Player *p[numPlayers];
			for (int i = 0 ; i < numPlayers; ++i){
				string arg;
				cin >> arg;
				if (arg == "human"){
					p[i] = new Human();
				} else {
					int lvl = arg[9] - '0';
					p[i] = new Ai(lvl, (bool)i, playerPieces[i]);
				}
			}

			runGame(p, curGame);

			for (int i = 0; i < numPlayers; ++i)
			{
				delete p[i];
			}

			delete curGame;
			curGame = new Game();

		} else {
			cerr << "Invalid Command! Please enter setup or game." << endl;
		}
	}
	delete curGame;
	cout << "Final Score:" << endl;
	cout << "White: " << score[0] << endl;
	cout << "Black: " << score[1] << endl;

}


void runGame(Player *p[], Game *curGame){
	cout << *curGame;

	int start = curGame->getStartPlayer();
	bool result = true;
	for (int moveCount = 0; result ; ++moveCount){
		Move m = p[(moveCount + start)%numPlayers]->getMove(curGame);
		int moveResult = curGame->executeMove(m);
		if (!moveResult){
			--moveCount;
			cerr << "Invalid Move" << endl;
			continue;
		}
		p[(moveCount + start + 1)%numPlayers]->removePiece(m.getDest());

		result = reactToState(moveResult,(moveCount + start)%numPlayers);
		cout << *curGame;
	}
}

//returns false when endgame state reached
bool reactToState(int state, int curPlayer){
	if (state == 3){ //check mate
		cout << "Checkmate! ";
	}
	if (state >= 2){ //any win (resign or checkmate)
		cout << (((bool)curPlayer == (bool)(state%2)) ? "Black" : "White") << " wins!" << endl;
	} else if (state == 1){ //stalemate
		cout << "Stalemate!" << endl;
	} else if (state == -1){} //normal
	else if (state == -2){ //check
		cout << (curPlayer ? "White" : "Black") << " is in check." << endl;
	}
	return (state < 0);
}
