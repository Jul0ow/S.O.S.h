# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS =

SRC = main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: main.o ast.o ast_print.o ../utils/list.o ../lexing/lexer.o
main.o: ast.h ast_print.h ../utils/list.h ../lexing/lexer.h
ast.o: 
ast_print.o: 
../utils/list.o: 
../lexing/lexer.o:

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END

