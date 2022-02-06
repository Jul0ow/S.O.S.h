# Makefile

CC = gcc

CPPFLAGS= -MMD -g
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lm

all: main cat pwd ls

main: main.o prefix_graph.o vector.o
cat: command/cat.o
pwd: command/pwd.o
ls: command/ls.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
