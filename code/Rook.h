#IFNDEF _ROOK_H_
#DEFINE _ROOK_H_
#include <vector>
#include <utility>
#include "Piece.h"

class Rook: public Piece {
public:
    Rook(bool colour);
    bool isValid(std::pair<int,int> co);
    //Takes in difference in position for the move returns the required relative empty spaces
    std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
};

#endif