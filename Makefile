
SRC = cash.cc commandline.cc parse.cc scan.cc
OBJ = ${SRC:.cc=.o} 
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -MMD -Wall -Wextra -g


${BIN}: ${OBJ} 
	${CXX} ${OBJ} -o ${BIN}

%.cc: %.l
	flex -o $@ $<

 %.cc: %.y
	bison -d -o $@ $<



run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP}

-include $(DEP)
