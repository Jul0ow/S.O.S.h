# Makefile

CC = gcc

CPPFLAGS= -g -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` -lm

all: main

main: main.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
