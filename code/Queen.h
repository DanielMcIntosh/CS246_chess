#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "Piece.h"

class Queen: public Piece{
public:
	Queen(bool c);
	bool isValid(std::pair<int,int> co) override;
	std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
    char getChar() override;
};

#endif	
