#IFNDEF _KNIGHT_H_
#DEFINE _KNIGHT_H_
#include <vector>
#include <utility>
#include "Piece.h"

class Knight: public Piece {
public:
    Knight(bool colour);
    bool isValid(std::pair<int,int> co) override;
    //Takes in difference in position for the move returns the required relative empty spaces
    std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co) override;
};

#endif