#include "Human.h"

Human::Human() {}

Human::getMove() {
	string test;
	do {
		cin >> test;
	} while(test != "move");

	string s;
	s = cin.getline();
	return Move(s);
}