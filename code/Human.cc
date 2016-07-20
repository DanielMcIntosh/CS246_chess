#include "Human.h"

Human::Human() {}

Human::getMove() {
	string test;
	do {
		test = cin.getLine();
	} while(test.substr(0, 4) != "move");

	return Move(test);
}