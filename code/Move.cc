#include <sstream>
#include "Move.h"

using namespace std;


Move::Move(pair<int,int> o, pair<int,int> d, char pr): origin{o}, destination {d}, pawnRep {pr}{}

Move::Move(string s){
	istringstream ss{s};
	char ox;
	int oy;
	char dx;
	int dy;
	char pr;
	ss >> ox;
	ss >> oy;
	ss >> dx;
	ss >> dy;
	ss >> pr;
	origin.first = ox - 'a';
	origin.second = oy - 1;
	destination.first = dx -'a';
	destination.second = dy - 1;
	pawnRep = pr;
}

pair<int,int> Move::getOrigin(){
	return origin;
}

pair<int,int> Move::getDest(){
	return destination;
}

char Move::getPawnRep(){
	return pawnRep;
}

//ostream &operator<<(ostream &out, const Move &m) {
//	out << "Origin: " << m.origin.first << "," << m.origin.second << endl;
//	out << "Destination: " << m.destination.first << "," << m.destination.second << endl;
//	if (m.pawnRep){
//		out << "Pawn Replacement: " << m.pawnRep << endl;
//	}
//	return out;
//}


