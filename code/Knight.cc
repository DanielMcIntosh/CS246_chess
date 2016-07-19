#include <vector>
#include <utility>
#include "Knight.h"

use namespace std;

Knight::Knight(bool colour): colour{colour} {};

char Knight::getChar() {colour ? return 'k' : return 'K';}

bool Knight::isValid(pair<int, int> co) {
    if(co.first == 1 || co.first == -1) {
        return (co.second == 2 || co.second == -2) 
    } else(co.first == 2 || co.first == -2) {
        return (co.second == 1 || co.second == -1);
    }
}

bool Knight::isValidMove(std::pair<int,int> co) {isValid(co);}
bool Knight::isValidCapture(std::pair<int,int> co) {isValid(co);}

vector<pair<int,int>> Knight::getMoveReq(pair<int,int> co) {
    vector<pair<int,int>> req = {};
    return req;
}