#ifndef __KING_H__
#define __KING_H__

#include "Piece.h"

class King : public Piece{
public:
	King(bool c);
	bool isValidMove(std::pair<int,int> co) override;
	bool isValidCapture(std::pair<int,int> co) override;
    //Takes in difference in position for the move returns the required relative empty spaces
	std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
    char getChar() override;
private:
	bool isValid(std::pair<int,int> co);
};

#endif
