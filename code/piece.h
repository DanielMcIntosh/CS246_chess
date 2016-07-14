#include <vector>
#include <utility>

class Piece {
	bool colour;
public:
    virtual Piece(bool)=0;
	bool getColour();
	virtual bool isValid(std::pair<int,int> pm)=0;
	virtual vector(std::pair<int,int>) getMoveReq(pair<int,int>)=0;
    
};