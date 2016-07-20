#include "Piece.h"

bool Piece::getColour() {return colour;};

pair<int, int> Piece::convertCoords(string &s)
{
	return make_pair(s[0]-'a', s[1]-'1');
}

Piece *constructPiece(char c)
{
	cerr << "fill in Piece::constructPiece" << endl;;
	return nullptr;
}
