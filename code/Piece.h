#ifndef __PIECE_H__
#define __PIECE_H__
#include <vector>
#include <utility>
#include <string>

class Piece {
protected:
	bool colour;
public:
	Piece(bool colour);
	bool getColour();
	virtual char getChar()=0;
	virtual bool isValidMove(std::pair<int,int> co)=0;
	virtual bool isValidCapture(std::pair<int,int> co)=0;
	virtual std::vector<std::pair<int,int>> getMoveReq(std::pair<int,int> co)=0;
    
    static std::pair<int, int> convertCoords(std::string &s);
    static Piece *constructPiece(char c);

    virtual ~Piece();
};

#endif