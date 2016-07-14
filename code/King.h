#ifndef __KING_H__
#define __KING_H__

#include "Piece.h"

class King : public Piece{
public:
	King(bool c);
	bool isValid(std::pair<int,int> co);
	std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co);
    char getChar();
}

#endif
