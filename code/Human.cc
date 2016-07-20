#include "Human.h"

Human::Human() {}

Human::getMove() {
	string test;
	do {
		cin >> test;
	} while(test != "move");

	return Move(test);
}