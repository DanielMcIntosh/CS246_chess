#ifndef _PAWN_H_
#define _PAWN_H_
#include <vector>
#include <utility>
#include "Piece.h"

class Pawn: public Piece {
    bool firstMove;
public:
    Pawn(bool colour);
    bool isValid(std::pair<int,int> co) override;
    //Takes in difference in position for the move returns the required relative empty spaces
    std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
    char getChar() override;
    void firstMoved();
};

#endif