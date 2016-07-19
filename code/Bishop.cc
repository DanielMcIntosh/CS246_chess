#include "Bishop.h"
#include <utility>

using namespace std;

#define sign(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))

Bishop::Bishop(bool c): colour{c} {}

bool Bishop::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
	if (x/y == 1 || x/y == -1){
		return true;
	}
	return false;
}

bool Bishop::isValidMove(std::pair<int,int> co) {isValid(co);}
bool Bishop::isValidCapture(std::pair<int,int> co) {isValid(co);}

vector<pair<int,int>> Bishop::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> moves;
	int x = co.first;
	int y = co.second;
    for(int i = 1; i < abs(x) || i < abs(y); i++) {
        pair<int,int> m(i*sign(x), i*sign(y));
        moves.emplace_back(m);
    }
	return moves;
}

char Bishop::getChar(){
	return colour? 'b' : 'B';
}