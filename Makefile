
SRC = cash.cc commandline.cc
OBJ = ${SRC:.cc=.o}
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -MMD -Wall -Wextra -g

${BIN}: ${OBJ}
	${CXX} ${OBJ} -o ${BIN}

run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP}

-include $(DEP)
