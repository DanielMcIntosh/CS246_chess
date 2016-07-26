#include <iostream>
#include <string>
#include "View.h"

using namespace std;

View::View() {};

void View::reactToTurn(int result, int player) {
    if(result == 3) {
        cout << "Checkmate! ";
    }
    if (result >= 2){ //any win (resign or checkmate)
		cout << (((bool)player == (bool)(result%2)) ? "Black" : "White") << " wins!" << endl;
	} else if (result == 1){ //stalemate
		cout << "Stalemate!" << endl;
	} else if (result == 0) { //incorrect move command
        cout << "Invalid Move! Please try again!" << endl;
    } else if (result == -1){ //normal
        cout << (((bool)player == (bool)(result%2)) ? "White" : "Black") << "\'s turn, please enter a move." << endl;
    } else if (result == -2){ //check
		cout << (player ? "White" : "Black") << " is in check." << endl;
	}
}

void View::printScore(int wScore, int bScore) {
    cout << "Final Score:" << endl;
    cout << "White: " << wScore << endl;
	cout << "Black: " << bScore << endl;
}

void View::boardCheck(bool valid) {
    if(valid) {
        cout << "Setup is valid and completed!" << endl;
    } else {
        cout << "Setup is invalid, please check again!" << endl;
    }
}

void View::welcome() {
    cout << "Welcome to Chess!" << endl;
    cout << "Please enter a command!" << endl;
}

