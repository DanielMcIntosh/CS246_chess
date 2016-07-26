#include <string>
#include <iostream>

class View {
    std::istream &in;
    std::ostream &out;
public:
    View(std::istream &in = std::cin, std::ostream &out = std::cout);
    template<typename T> View &operator>>(T &a);
	template<typename T> View &operator<<(T &a);

    void reactToTurn(int result, int player);
    void printScore(int wScore, int bScore);
    void boardCheck(bool valid);
    void welcome();
};