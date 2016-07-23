#include <iostream>

class View {
    std::istream &in;
    std::istream &out;
public:
    View(std::istream &in, std::ostream &out);
    friend std::istream& operator>>(std::istream &is);
    friend std::ostream& operator<<(std::ostream &os);
    void reactToTurn(int result, int player);
    void printScore(int wScore, int bScore);
    void boardCheck(bool valid);
    void welcome();
}