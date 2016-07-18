#include "Queen.h"
#include <utility>

using namespace std;

Queen::Queen(bool c): colour{c} {}

bool Queen::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
    if (x == 0) {
        if(y != 0) {
            return true;
        } else {
            return false;
        }
    } else if (y == 0) {
        if(x != 0) {
            return true;
        } else {
            return false;
        }
    } else if (x/y == 1 || x/y == -1){
		return true;
	}
	return false;
}


vector<pair<int,int>> Queen::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> moves;
	int x = co.first;
	int y = co.second;
	if (y == 0) {
        for(int i = 1; i < x; i++) {
            pair<int,int> m(i, 0);
            moves.emplace_back(m);
        } 
    } else if (x == 0) {
        for(int i = 1; i < y; i++) {
            pair<int,int> m(0, i);
            moves.emplace_back(m);
        }
    } else if (x > 0 && y > 0){
		for (int i = 1; i < x; ++i){
			pair<int,int> m (i,i);
			moves.emplace_back(m);
		}
	} else if (x > 0 && y < 0){
		for (int i = 1; i < x; ++i){
			pair<int,int> m (i,i*(-1));
			moves.emplace_back(m);
		}
	} else if (x < 0 && y < 0){
		for (int i = -1; i > x; --i){
			pair<int,int> m (i,i);
			moves.emplace_back(m);
		}
	} else {
		for (int i = -1; i > x; --i){
			pair<int,int> m (i,(-1)*i);
			moves.emplace_back(m);
		}
	}
	return moves;
}

char Queen::getChar(){
	return colour? 'q' : 'Q';
}
