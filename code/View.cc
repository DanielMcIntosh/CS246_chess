#include <iostream>
#include <string>
#include "View.h"

using namespace std;

View::View(istream &in=cin, ostream &out=cout): in{in}, out{out} {};

View::istream& operator>>(std::istream &is) {
    in >> is;
    return is;
}

View::ostream& operator<<(std::ostream &os) {
    
}

void View::reactToTurn(int result, int player) {
    if(result == 3) {
        out << "Checkmate! ";
    }
    if (result >= 2){ //any win (resign or checkmate)
		out << (((bool)player == (bool)(result%2)) ? "Black" : "White") << " wins!" << endl;
	} else if (result == 1){ //stalemate
		out << "Stalemate!" << endl;
	} else if (result == 0) { //incorrect move command
        out << "Invalid Move! Please try again!" << endl;
    } else if (result == -1){ //normal
        out << (((bool)player == (bool)(result%2)) ? "White" : "Black") << "\'s turn, please enter a move." << endl;
    } else if (result == -2){ //check
		out << (player ? "White" : "Black") << " is in check." << endl;
	}
}

void View::printScore(int wScore, int bScore) {
    out << "Final Score:" << endl;
    out << "White: " << wScore << endl;
	out << "Black: " << bScore << endl;
}

void View::boardCheck(vool valid) {
    if(valid) {
        out << "Setup is valid and completed!" << endl;
    } else {
        out << "Setup is invalid, please check again!" << endl;
    }
}

void View::welcome() {
    out << "Welcome to Chess!" << endl;
    out << "Please enter a command!" << endl;
}

