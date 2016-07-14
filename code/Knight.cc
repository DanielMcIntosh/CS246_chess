#include <vector>
#include <utility>
#include "Knight.h"

use namespace std;

Knight::Knight(bool colour): colour{colour} {};

char Knight::getChar() {colour ? return 'k' : return 'K';};

bool Knight::isValid(pair<int, int> co) {
    if(co.first == 1 || co.first == -1) {
        if(co.second == 2 || co.second == -2) {
            return true;
        } else {
            return false;
        }
    } else(co.first == 2 || co.first == -2) {
        if(co.second == 1 || co.second == -1) {
            return true;
        } else {
            return false;
        }
    }
}

vector<pair<int,int>> Pawn::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    return req;
}