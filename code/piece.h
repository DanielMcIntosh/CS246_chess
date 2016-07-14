#IFNDEF _PIECE_H
#include <vector>
#include <utility>

class Piece {
	bool colour;
public:
    virtual Piece()=0;
	bool getColour();
	virtual bool isValid(std::pair<int,int> co)=0;
	virtual std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co)=0;
    virtual char getChar()=0;
};

#endif