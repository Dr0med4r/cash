
SRC = parse.cc scan.cc cash.cc commandline.cc shellcall.cc
OBJ = ${SRC:.cc=.o} 
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -std=c++20 -MMD -Wall -Wextra -g
COMPILEFLAGS = -lreadline

all: ${BIN}


.SUFFIXES: .cc .c .o



parse.cc parse.hh: parse.y
	bison -d $<

scan.cc: scan.l
	flex -o $@ $<


%.c: %.y

%.o: %.c
	
${BIN}: ${OBJ} 
	${CXX} ${COMPILEFLAGS} ${OBJ} -o ${BIN}



run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP} parse.hh parse.cc scan.cc

-include $(DEP)
