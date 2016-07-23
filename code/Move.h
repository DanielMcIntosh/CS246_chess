#ifndef __MOVE_H__
#define __MOVE_H__
#include <utility>
#include <string>

class Move{
	std::pair<int,int> origin;
	std::pair<int,int> destination;
	char pawnRep ='\0';
	bool colour;
public:
	Move(std::pair<int,int> o, std::pair<int,int> d, char pr, bool colour);
	Move(std::string s, bool colour);
	std::pair<int,int> getOrigin();
	std::pair<int,int> getDest();
	char getPawnRep();
	bool getColour();
	//friend std::ostream &operator<<(std::ostream &out, const Move &m);
};

#endif
