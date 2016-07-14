#IFNDEF _PAWN_H
#include <vector>
#include <utility>
#include "Piece.h"

class Pawn: public Piece {
public:
    Pawn(bool colour);
    bool isValid(std::pair<int,int> pm);
    //Takes in difference in position for the move returns the required relative empty spaces
    vector vector(std::pair<int,int>) getMoveReq();
};

#endif