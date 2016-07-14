#include <utility>
#include "King.h"

using namespace std;


char King::getChar(){
	return colour ? 'k' : 'K';
}

King::King(bool c): colour{c}{}

bool King::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
	if ((x >= -1 && x <= 1) && (y >= -1 & y <= 1)){
		return true;
	}
	return false;
}

vector<pair<int,int>> King::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> path;
	return path;
}
