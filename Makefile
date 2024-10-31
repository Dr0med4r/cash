
SRC = parse.cc scan.cc cash.cc commandline.cc
OBJ = ${SRC:.cc=.o} 
DEP = $(SRC:.cc=.d)
BIN = cash


CXXFLAGS= -MMD -Wall -Wextra -g -O0

all: ${BIN}


.SUFFIXES: .cc .c .o



scan.cc: scan.l
	flex -o $@ $<

 %.cc %.hh: %.y
	bison  $<

%.c: %.y

%.o: %.c
	
${BIN}: ${OBJ} 
	${CXX} ${OBJ} -o ${BIN}



run: ${BIN}
	./${BIN}

clean: 
	rm -f ${OBJ} ${DEP} parse.hh parse.cc scan.cc

-include $(DEP)
