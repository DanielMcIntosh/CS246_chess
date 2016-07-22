#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "Piece.h"

class Queen: public Piece{
public:
	Queen(bool c);
    char getChar() override;
	bool isValidMove(std::pair<int,int> co) override;
	bool isValidCapture(std::pair<int,int> co) override;
    //Takes in difference in position for the move returns the required relative empty spaces
	std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
private:
	bool isValid(std::pair<int,int> co);
};

#endif	
