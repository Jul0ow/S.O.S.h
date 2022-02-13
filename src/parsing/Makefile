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

main: main.o ast.o ast_print.o listT.o lexer.o xmalloc.o
main.o: ast.h ast_print.h listT.h lexer.h
ast.o: xmalloc.h lexer.h listT.h
ast_print.o: ast.h ast_print.h
listT.o: lexer.h xmalloc.h
lexer.o: listT.h vectorS.h lexer.h xmalloc.h
xmalloc.o: xmalloc.h

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END

