#include "Player.h"
#include "Game.h"
#include "Move.h"
#include <iostream>
#include <vector>
#include <utility>
#define numPlayers 2

using namespace std;

int main(){
	Game * curGame = new Game{}; // need edit
	int score [numPlayers] = {0};
	string input;
	while (cin>>input){
		if (input == "setup"){
			curGame->setup();
		} else if (input == "game"){
			Player p[numPlayers];
			for (int i = 0 ; i < numPlayers; ++i){
				string arg;
				cin >> arg;
				if (arg == "human"){
					p[i] = Human(); 
				} else {
					int lvl = arg[9] - '0';
					p[i] = Ai(lvl);
				}
			}
			runGame(p);
		} else {
			cerr << "Invalid Command! Please enter setup or game." << endl;
		}
	}
}


void runGame(Player p[]){
	int start = curGame.getStartPlayer();
	for (int moveCount = 0, bool result =true ; result ; ++moveCount){
		Move m = player[(moveCount + start)%numPlayers].getMove();
		int moveResult = curGame.executeMove(m);
		result = reactToState(moveResult,(moveCount + start)%numPlayers);
	}
}

bool reactToState(int i, int curPlayer){
	if (i == 1){
		cout << "Checkmate! " << (curPlayer ? "Black" : "White") << " wins!" << endl;
	}

}





