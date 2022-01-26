CC= gcc
CFLAGS= -Wall -Werror -Wextra -O1 -pedantic -std=c99 -g -fsanitize=address

SRC= $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ= $(patsubst %.c,%.o,$(SRC))
HEA= $(addprefix -I, $(dir $(wildcard src/*/)))

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(HEA) -o main



%.o: %.c
	$(CC) $(CFLAGS) $(HEA) -c $< -o $@


clean:
	$(RM) $(OBJ)
	$(RM) main
