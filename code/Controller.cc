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
			runGame();
		} else {
			cerr << "Invalid Command! Please enter setup or game." << endl;
		}
	}
}


void runGame(){
	int moveCount = 0;
	
}