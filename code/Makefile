CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla -ggdb
EXEC = foo
OBJECTS = Controller.o Game.o Piece.o Move.o Player.o Human.o Ai.o King.o Pawn.o Queen.o Bishop.o Rook.o Knight.o View.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
