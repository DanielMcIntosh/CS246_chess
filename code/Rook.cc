#include <vector>
#include <utility>
#include "Rook.h"

use namespace std;

Rook::Rook(bool colour): colour{colour} {};

char Rook::getChar() {colour ? return 'r' : return 'R';};

bool Rook::isValid(pair<int, int> co) {
    
}

vector<pair<int,int>> Pawn::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    return req;
}