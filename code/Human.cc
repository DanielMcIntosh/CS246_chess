#include "Human.h"
#include "Move.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

Human::Human(bool colour): Player{colour}{}

Move Human::getMove(Game *g) {
	string test;
	do {
		cin >> test;
	} while(test != "move" && test != "resign");

	if (test == "move")
	{
		string s;
		getline(cin, s);
		istringstream ss {s};
		string origin;
		string dest;
		ss >> origin;
		ss >> dest;
		if (origin == dest)
		{
			cerr << "Invalid move: origin and destination are the same!";
		}
		return Move(s, colour);
	}
	else
	{
		return Move(make_pair(0, 0), make_pair(0, 0), 0, colour);
	}
}

void Human::removePiece(std::pair<int, int> piece){}
