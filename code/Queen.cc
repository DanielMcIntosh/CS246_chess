#include "Queen.h"
#include <utility>

using namespace std;

#define sign(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))

Queen::Queen(bool c): Piece{c} {}

bool Queen::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
    return ((x || y) && (!x || !y || abs(x) == abs(y)));
}

bool Queen::isValidMove(std::pair<int,int> co) {return isValid(co);}
bool Queen::isValidCapture(std::pair<int,int> co) {return isValid(co);}

vector<pair<int,int>> Queen::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> moves;
	int x = co.first;
	int y = co.second;
    for(int i = 1; i < abs(x) || i < abs(y); i++) {
        pair<int,int> m(i*sign(x), i*sign(y));
        moves.emplace_back(m);
    }
	return moves;
}

char Queen::getChar(){
	return colour ? 'q' : 'Q';
}
