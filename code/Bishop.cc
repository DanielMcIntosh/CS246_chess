#include "Bishop.h"
#include <utility>


Bishop::Bishop(bool c): colour{c} {}

bool Bishop::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
	if (x/y == 1 || x/y == -1){
		return true;
	}
	return false;
}


vector<pair<int,int>> Bishop::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> moves;
	int x = co.first;
	int y = co.second;
	if (x > 0 && y > 0){
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

char Bishop::getChar(){
	return colour? 'b' : 'B';
}