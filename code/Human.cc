#include "Human.h"

Human::Human() {}

Human::getMove() {
	string test;
	do {
		cin >> test;
	} while(test != "move" && test != "resign");

	if (test == "move")
	{
		string s;
		s = cin.getline();

		if (s.substr(0, 2) == s.substr(3, 2))
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