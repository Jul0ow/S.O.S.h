CC= gcc
CFLAGS= -Wall -Werror -Wextra -O1 -pedantic -std=c99 -g -fsanitize=address

ALL= $(wildcard src/*.c) $(wildcard src/*/*.c)

TEST_SRC= $(filter-out $(wildcard src/UI/*.c), $(ALL))
SRC= $(filter-out $(wildcard src/testSuite/*.c), $(ALL))
TMP= $(filter-out $(wildcard src/parsing/*.c), $(SRC))
PRINT_SRC= $(filter-out $(wildcard src/UI/*.c), $(TMP))


OBJ= $(patsubst %.c,%.o,$(SRC))
TEST_OBJ= $(patsubst %.c,%.o,$(TEST_SRC))
PRINT_OBJ = $(patsubst %.c,%.o,$(PRINT_SRC))

HEA= $(addprefix -I, $(dir $(wildcard src/*/)))
HEAP= $(filter-out $(wildcard src/UI/*.c), $(HEA))


all: main print test

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(HEA) -o main

print: $(PRINT_OBJ)
	$(CC) $(CFLAGS) $(PRINT_OBJ) $(HEAP) -o lexing

test: CFLAGS += -lcriterion
test: $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(HEA) -o test

 

%.o: %.c
	$(CC) $(CFLAGS) $(HEAP) -c $< -o $@


clean:
	$(RM) $(OBJ)
	$(RM) main test lexing
