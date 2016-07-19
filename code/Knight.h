#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include <vector>
#include <utility>
#include "Piece.h"

class Knight: public Piece {
public:
    Knight(bool colour);
	bool isValidMove(std::pair<int,int> co) override;
	bool isValidCapture(std::pair<int,int> co) override;
    //Takes in difference in position for the move returns the required relative empty spaces
    std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
private:
	bool isValid(std::pair<int,int> co);
};

#endif