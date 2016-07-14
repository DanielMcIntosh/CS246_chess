#include <piece.h>


class Game {
	Piece * board[8][8];
public:
	Game();
	int executeMove(Move &m);
	int tryMove(Move &m);
	bool isValidBoard();
	void setup();
};