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

main: main.o listT.o lexer.o xmalloc.o vectorS.o ast.o #ast_print.o
main.o:main.c  ast.h listT.h lexer.h #ast_print.h 
ast.o:ast.c xmalloc.h lexer.h listT.h
#ast_print.o:ast_print.c ast.h ast_print.h
listT.o: listT.c lexer.h xmalloc.h listT.h
lexer.o: lexer.c listT.h vectorS.h lexer.h xmalloc.h
vectorS.o: vectorS.c vectorS.h xmalloc.h
xmalloc.o: xmalloc.c xmalloc.h

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END

