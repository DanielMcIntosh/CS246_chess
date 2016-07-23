#include "Piece.h"

#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"

using namespace std;

Piece::Piece(bool c): colour{c}, firstMoved{true} {}

bool Piece::getColour() {return colour;};

bool Piece::firstMoved() {
    if(firstMoved) {
        firstMoved = false;
        return true;
    }
    return false;
}

pair<int, int> Piece::convertCoords(string &s)
{
	return make_pair(s[0]-'a', s[1]-'1');
}

Piece *Piece::constructPiece(char c)
{
    bool colour = c & ('a' - 'A');
	c |=  ('a' - 'A');

    if(c == 'r') {
        return new Rook(colour);
    } else if(c == 'n') {
        return new Knight(colour);
    } else if(c == 'b') {
        return new Bishop(colour);
    } else if(c == 'q') {
        return new Queen(colour);
    } else if(c == 'k') {
        return new King(colour);
    } else if(c == 'p') {
        return new Pawn(colour);
    }
    return nullptr;
}

Piece::~Piece() {}
