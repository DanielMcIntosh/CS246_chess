#include <vector>
#include <utility>
#include "Pawn.h"

use namespace std;

Pawn::Pawn(bool colour): colour{colour}, firstMove{true} {};

char Pawn::getChar() {colour ? return 'p' : return 'P';};

bool Pawn::isValid(pair<int, int> co) {
    if ((co.second == 1 || co.second == 2) && co.first == 0) {
        if (co.second == 2) {
            firstMove ? return true : return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

vector<pair<int,int>> Pawn::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    if (co.second == 2) {
        pair<int, int> temp(0, 1);
        req.emplace(temp);
    }
    return req;
}

void Pawn::firstMoved() {
    this.firstMove = false;
}