
SRC = cash.cc commandline.cc parse.cc
OBJ = ${SRC:.cc=.o} scan.o
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -MMD -Wall -Wextra -g


${BIN}: ${OBJ} 
	${CXX} ${OBJ} -o ${BIN}

%.c: %.l
	flex -o $@ $<

 %.hh %.cc: %.y
	bison -dL c++ -o $@ $<



run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP}

-include $(DEP)
