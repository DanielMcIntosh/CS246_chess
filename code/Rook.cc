#include <vector>
#include <utility>
#include "Rook.h"

use namespace std;

Rook::Rook(bool colour): colour{colour} {};

char Rook::getChar() {colour ? return 'r' : return 'R';};

bool Rook::isValid(pair<int, int> co) {
    if(co.first == 0) {
        return (co.second != 0);
    } else if(co.second == 0) {
        return (co.first != 0);
    }
}

bool Rook::isValidMove(std::pair<int,int> co) {isValid(co);}
bool Rook::isValidCapture(std::pair<int,int> co) {isValid(co);}

vector<pair<int,int> > Rook::getMoveReq(pair<int,int> co) {
    vector<pair<int,int> > req = {};
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