# Makefile

CC = gcc

CPPFLAGS= -g -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lm

all: main cat pwd ls

main: main.o
cat: cat.o
pwd: pwd.o
ls: ls.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
