#include <sstream>
#include "Move.h"
#include "Piece.h"

using namespace std;


Move::Move(pair<int,int> o, pair<int,int> d, char pr, bool c): origin{o}, destination {d}, pawnRep {pr}, colour{c}{}

Move::Move(string s, bool colour): colour{colour}{
	string sOrigin;
	string sDest;
	istringstream ss{s};
	ss >> sOrigin;
	ss >> sDest;
	origin = Piece::convertCoords(sOrigin);
	destination = Piece::convertCoords(sDest);
	ss >> pawnRep;
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

bool Move::getColour(){
	return this->colour;
}

//ostream &operator<<(ostream &out, const Move &m) {
//	out << "Origin: " << m.origin.first << "," << m.origin.second << endl;
//	out << "Destination: " << m.destination.first << "," << m.destination.second << endl;
//	if (m.pawnRep){
//		out << "Pawn Replacement: " << m.pawnRep << endl;
//	}
//	return out;
//}


