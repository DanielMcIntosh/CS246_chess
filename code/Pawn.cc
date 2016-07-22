#include <vector>
#include <utility>
#include "Pawn.h"

#include <iostream>

using namespace std;

Pawn::Pawn(bool c): Piece{c}, firstMove{true} {}

char Pawn::getChar() {return colour ? 'p' : 'P';};

bool Pawn::isValidMove(pair<int, int> co) {
    
    #ifdef inDebug
    cout << "pawn 1" << endl;
    cout << "first move = " << firstMove << endl;
    cout << "colour = " << colour << endl;
    #endif
    
    int dir = 1;
    if (this->colour) { dir = -1;}

    if ((co.second == 1*dir || co.second == 2*dir) && co.first == 0) {
        if (co.second == 2*dir) {
            return firstMove;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

bool Pawn::isValidCapture(pair<int, int> co) {
    int dir = 1;
    if (this->colour) { dir = -1;}

    return (co.second == 1*dir && abs(co.first) == 1);
}

vector<pair<int,int>> Pawn::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    if (abs(co.second) == 2) {
        pair<int, int> temp(0, 1);
        req.emplace_back(temp);
    }
    return req;
}

void Pawn::firstMoved() {
    this->firstMove = false;
}