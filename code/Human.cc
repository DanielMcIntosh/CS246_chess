#include "Human.h"
#include <string>
#include <sstream>

using namespace std;

Human::Human() {}

Move Human::getMove(Game *g) {
	string test;
	do {
		cin >> test;
	} while(test != "move" && test != "resign");

	if (test == "move")
	{
		string s;
		s = cin.getline();
		istringstream ss {s};
		string origin;
		string dest;
		ss >> origin;
		ss >> dest;
		if (origin == dest)
		{
			cerr << "Invalid move: origin and destination are the same!";
		}
		return Move(s);
	}
	else
	{
		return Move(make_pair(0, 0), make_pair(0, 0), 0);
	}
}