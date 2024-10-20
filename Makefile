
SRC = cash.cc commandline.cc scan.c parse.cc
OBJ = ${SRC:.cc=.o}
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -MMD -Wall -Wextra -g


%.c: %.l
	flex $< -o $@

%.cc: %.y
	bison -dL c++ $@ $<


${BIN}: ${OBJ} 
	${CXX} ${OBJ} -o ${BIN}

run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP}

-include $(DEP)
