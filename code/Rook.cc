#include <vector>
#include <utility>
#include "Rook.h"

using namespace std;

Rook::Rook(bool c): Piece{c} {}

char Rook::getChar() {return colour ? 'r' : 'R';}

bool Rook::isValid(pair<int, int> co) {
    if(co.first == 0) {
        return (co.second != 0);
    } else if(co.second == 0) {
        return (co.first != 0);
    }
    return false;
}

bool Rook::isValidMove(std::pair<int,int> co) {return isValid(co);}
bool Rook::isValidCapture(std::pair<int,int> co) {return isValid(co);}

vector<pair<int,int> > Rook::getMoveReq(pair<int,int> co) {
    vector<pair<int,int> > req = {};
    if(co.second == 0) {
        for(int i = 1; i < co.first; i++) {
            pair<int,int> temp(i, 0);
            req.emplace_back(temp);
        } 
    } else {
        for(int i = 1; i < co.second; i++) {
            pair<int,int> temp(0, i);
            req.emplace_back(temp);
        }
    }
    return req;
}