#include <vector>
#include <utility>
#include "Rook.h"

using namespace std;

#define sign(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))


Rook::Rook(bool c): Piece{c} {}

char Rook::getChar() {return colour ? 'r' : 'R';}

bool Rook::isValid(pair<int, int> co) {
    return ((!co.first) != (!co.second));
}

bool Rook::isValidMove(std::pair<int,int> co) {return isValid(co);}
bool Rook::isValidCapture(std::pair<int,int> co) {return isValid(co);}

vector<pair<int,int> > Rook::getMoveReq(pair<int,int> co) {
    vector<pair<int,int> > req{};
    if(co.second == 0) {
        for(int i = 1; i < abs(co.first); i++) {
            pair<int,int> temp(i*sign(co.first), 0);
            req.push_back(temp);
        } 
    } else {
        for(int i = 1; i < abs(co.second); i++) {
            pair<int,int> temp(0, i*sign(co.second));
            req.push_back(temp);
        }
    }
    return req;
}