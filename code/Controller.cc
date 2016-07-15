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
	cout << "Final Score:" << endl;
	cout << "White: " << score[0] << endl;
	cout << "Black: " << score[1] << endl;
}


void runGame(Player p[]){
	int start = curGame.getStartPlayer();
	for (int moveCount = 0, bool result =true ; result ; ++moveCount){
		Move m = player[(moveCount + start)%numPlayers].getMove();
		int moveResult = curGame.executeMove(m);
		if (!moveResult){
			--moveCount;
			cerr << "Invalid Move" << endl;
			continue;
		}
		result = reactToState(moveResult,(moveCount + start)%numPlayers);
	}
}

bool reactToState(int i, int curPlayer){
	if (i == 3){
		cout << "Checkmate! ";
	}
	if (i >= 2){
		cout << (((bool)curPlayer == (bool)(i%2)) ? "Black" : "White") << " wins!" << endl;
		return false;
	} else if (i == 1){
		cout << "Stalemate!" << endl;
		return false;
	} else if (i == -1){
		return true;
	} else {
		cout << (curPlayer ? "White" : "Black") << " is in check." << endl;
		return true;
	}
}







