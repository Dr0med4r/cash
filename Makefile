
SRC = parse.cc scan.cc cash.cc commandline.cc builtins.cc call.cc main.cc cd.cc kill.cc alias.cc pwd.cc
OBJ = ${SRC:.cc=.o} 
DEP = $(SRC:.cc=.d)
BIN = cash
VPATH = src


CXXFLAGS= -std=c++20 -MMD -Wall -Wextra -g -Isrc
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
	rm -f ${BIN} ${OBJ} ${DEP} parse.hh parse.cc scan.cc

-include $(DEP)
