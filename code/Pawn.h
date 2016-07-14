#IFNDEF _PAWN_H_
#DEFINE _PAWN_H_
#include <vector>
#include <utility>
#include "Piece.h"

class Pawn: public Piece {
    bool firstMove;
public:
    Pawn(bool colour);
    bool isValid(std::pair<int,int> co);
    //Takes in difference in position for the move returns the required relative empty spaces
    std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co);
    char getChar();
    void firstMoved();
};

#endif