
SRC = cash.cc
OBJ = ${SRC:.cc=.o}
BIN = cash


CXXFLAGS= -Wall -Wextra

${BIN}: ${OBJ}
	${CXX} ${OBJ} -o ${BIN}

run: ${BIN}
	./${BIN}

clean: 
	rm ${OBJ}
