#include <vector>
#include <utility>
#include "Rook.h"

use namespace std;

Rook::Rook(bool colour): colour{colour} {};

char Rook::getChar() {colour ? return 'r' : return 'R';};

bool Rook::isValid(pair<int, int> co) {
    if(co.first == 0) {
        if(co.second != 0) {
            return true;
        } else {
            return false;
        }
    } else if(co.second == 0) {
        if(co.first != 0) {
            return true;
        } else {
            return false;
        }
    }
}

vector<pair<int,int>> Pawn::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    if(co.second == 0) {
        for(int i = 1; i < co.first; i++) {
            pair<int,int> temp(i, 0);
            req.emplace(temp);
        } 
    } else {
        for(int i = 1; i < co.second; i++) {
            pair<int,int> temp(0, i);
            req.emplace(temp);
        }
    }
    return req;
}