#include <utility>

class Move{
	std::pair<int,int> origin;
	std::pair<int,int> destination;
	char pawnRep;
public:
	Move(std::pair<int,int> o, std::pair<int,int> d);
	Move(std::string s);
}