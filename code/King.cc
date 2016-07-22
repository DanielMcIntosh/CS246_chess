#include <utility>
#include "King.h"

using namespace std;


char King::getChar(){
	return colour ? 'k' : 'K';
}

King::King(bool c): Piece{c} {}

bool King::isValid(pair<int,int> co){
	int x = co.first;
	int y = co.second;
	return ((x || y) && (x >= -1 && x <= 1) && (y >= -1 && y <= 1));
}

bool King::isValidMove(std::pair<int,int> co) {return isValid(co);}
bool King::isValidCapture(std::pair<int,int> co) {return isValid(co);}

vector<pair<int,int>> King::getMoveReq(pair<int,int> co){
	vector<pair<int,int>> path = {};
	return path;
}

