
SRC = cash.cc
OBJ = ${SRC:.cc=.o}
BIN = cash


CXXFLAGS= -Wall -Wextra -g

${BIN}: ${OBJ}
	${CXX} ${OBJ} -o ${BIN}

run: ${BIN}
	./${BIN}

clean: 
	rm ${OBJ}
