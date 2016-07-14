#ifndef __BISHOP_H__
#define __BISHOP_H__

#include "Piece.h"

class Bishop: public Piece{
public:
	Bishop(bool c);
	bool isValid(std::pair<int,int> co) override;
	std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
    char getChar() override;
};

#endif	
