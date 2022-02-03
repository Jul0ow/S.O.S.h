CC= gcc
CFLAGS= -Wall -Werror -Wextra -O1 -pedantic -std=c99 -g -fsanitize=address

TEST_SRC= $(wildcard src/*.c) $(wildcard src/*/*.c) 
SRC = $(filter-out $(wildcard src/testSuite/*.c), $(TEST_SRC))

OBJ= $(patsubst %.c,%.o,$(SRC))
TEST_OBJ= $(patsubst %.c,%.o,$(TEST_SRC))
HEA= $(addprefix -I, $(dir $(wildcard src/*/)))

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(HEA) -o main

test: CFLAGS += -lcriterion
test: $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(HEA) -o test

 

%.o: %.c
	$(CC) $(CFLAGS) $(HEA) -c $< -o $@


clean:
	$(RM) $(OBJ)
	$(RM) main
