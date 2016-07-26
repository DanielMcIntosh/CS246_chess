#ifndef _VIEW_
#define _VIEW_
#include <string>
#include <iostream>

class View {
public:
    View();
    void reactToTurn(int result, int player);
    void printScore(int wScore, int bScore);
    void boardCheck(bool valid);
    void welcome();
};

#endif