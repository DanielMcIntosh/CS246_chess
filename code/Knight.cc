#include <vector>
#include <utility>
#include "Knight.h"

using namespace std;

Knight::Knight(bool c): Piece{c} {}

char Knight::getChar() {return colour ? 'n' : 'N';}

bool Knight::isValid(pair<int, int> co) {
    if(co.first == 1 || co.first == -1) {
        return (co.second == 2 || co.second == -2);
    } else if (co.first == 2 || co.first == -2) {
        return (co.second == 1 || co.second == -1);
    }
    return false;
}

bool Knight::isValidMove(std::pair<int,int> co) {return isValid(co);}
bool Knight::isValidCapture(std::pair<int,int> co) {return isValid(co);}

vector<pair<int,int>> Knight::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    return req;
}