CC      := clang
CFLAGS  := -std=c99 -g -Wall -Wextra

TEST_SRC := test/main.c
TEST_TARGET := test/tests

all: test

test:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET) -ldl
	./$(TEST_TARGET)

clean:
	rm -f $(TEST_TARGET)

.PHONY: all test clean
