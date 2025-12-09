# Compiler settings
CC = gcc
CFLAGS = -Wall -g

# Source directory
SRC = src
TESTS = tests

# Object files (excluding main.o for tests)
OBJS = $(SRC)/main.o $(SRC)/builtins.o $(SRC)/parser.o $(SRC)/utility.o $(SRC)/executor.o
TEST_OBJS = $(SRC)/builtins.o $(SRC)/parser.o $(SRC)/utility.o $(SRC)/executor.o $(TESTS)/test_suite.o

# Output binary
.PHONY: shell test clean

shell: $(OBJS)
	$(CC) $(CFLAGS) -o shell $(OBJS)

# Compilation rules
$(SRC)/main.o: $(SRC)/main.c $(SRC)/parser.h $(SRC)/builtins.h
	$(CC) $(CFLAGS) -c $(SRC)/main.c -o $(SRC)/main.o

$(SRC)/parser.o: $(SRC)/parser.c $(SRC)/parser.h
	$(CC) $(CFLAGS) -c $(SRC)/parser.c -o $(SRC)/parser.o

$(SRC)/builtins.o: $(SRC)/builtins.c $(SRC)/builtins.h $(SRC)/parser.h
	$(CC) $(CFLAGS) -c $(SRC)/builtins.c -o $(SRC)/builtins.o

$(SRC)/utility.o: $(SRC)/utility.c $(SRC)/utility.h
	$(CC) $(CFLAGS) -c $(SRC)/utility.c -o $(SRC)/utility.o

$(SRC)/executor.o: $(SRC)/executor.c $(SRC)/executor.h
	$(CC) $(CFLAGS) -c $(SRC)/executor.c -o $(SRC)/executor.o

$(TESTS)/test_suite.o: $(TESTS)/test_suite.c $(TESTS)/test.h
	$(CC) $(CFLAGS) -I. -c $(TESTS)/test_suite.c -o $(TESTS)/test_suite.o

# Test target
test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o test_runner $(TEST_OBJS)
	./test_runner

clean:
	rm -f $(SRC)/*.o $(TESTS)/*.o shell test_runner