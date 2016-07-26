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
#include "window.h"

#define numPlayers 2

using namespace std;

int main(){
	View v = View();
	v.welcome();
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

	while (cin >> input){
		if (input == "setup"){
			curGame->setup(playerPieces);
			for(int i = 0 ; i < 2; ++i){
				v << i << endl;
				for(auto n:playerPieces[i]){
					cout << n.first << ", " << n.second << endl;
				}
			}
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

			int winner = runGame(p, curGame);
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
	cout << "Final Score:" << endl;
	cout << "White: " << score[0] << endl;
	cout << "Black: " << score[1] << endl;

}


int runGame(Player *p[], Game *curGame){
	cout << *curGame;
	Xwindow display(225,225);
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

		result = reactToState(moveResult,(moveCount + start)%numPlayers);
		cout << *curGame;
	}
	if (result < 0)
		return -1;
	return result % 2;
}

//returns false when endgame state reached
int reactToState(int state, int curPlayer){
	if (state == 3){ //check mate
		cout << "Checkmate! ";
	}
	if (state >= 2){ //any win (resign or checkmate)
		cout << (((bool)curPlayer == (bool)(state%2)) ? "Black" : "White") << " wins!" << endl;
	} else if (state == 1){ //stalemate
		cout << "Stalemate!" << endl;
		return -1;
	} else if (state == -1){} //normal
	else if (state == -2){ //check
		cout << (curPlayer ? "White" : "Black") << " is in check." << endl;
	} 
	return (state > 0) ? ((curPlayer + state) % 2 + 1) : 0;
}


void displaySetup(Xwindow *x, Game * g){
	for (int i = 0; i < 9 ; ++i){
    if (i == 8){
      for(int j = 0; j < 8; ++j){
        string s = to_string(8-j);
        x->drawString(i*25+10, j*25+16, s, Xwindow::Black);
      }
    } else {
      for(int j=0; j < 9 ; ++j){
        if (j == 8){
          char c = i + 'a';
          stringstream ss;
          string s;
          ss << c;
          ss >> s;
          x.drawString(i*25+10, j*25+16, s, Xwindow::Black);
        } else {
          if (i % 2 == j%2){
            char c = g->getPosChar(i,7-j);
            x->fillRectangle(i*25, j*25, 25, 25, Xwindow::White);
            if (c == '_'){
              continue;
            }
            if (c < 'a'){
	          stringstream ss;
	          string s;
	          ss << c;
	          ss >> s;
              x->drawString(i*25+10, j*25+16, s , Xwindow::Magenta);
            } else {
	          stringstream ss;
	          string s;
	          ss << c;
	          ss >> s;
              s[0] -= 32;
              x->drawString(i*25+10, j*25+16, s, Xwindow::Orange);
            }
          } else {
            char c = g->getPosChar(i,7-j);
            x->fillRectangle(i*25, j*25, 25, 25, Xwindow::Black);
            if (c == '_'){
              continue;
            }
            if (c < 'a'){
	          stringstream ss;
	          string s;
	          ss << c;
	          ss >> s;
              x->drawString(i*25+10, j*25+16, s , Xwindow::Magenta);
            } else {
	          stringstream ss;
	          string s;
	          ss << c;
	          ss >> s;
              s[0] -= 32;
              x->drawString(i*25+10, j*25+16, s, Xwindow::Orange);
            }
          }
        }
      }
    }
  }
}