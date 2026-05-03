UNAME_S := $(shell uname -s)
CC      ?= cc
CFLAGS  := -std=c99 -g -Wall -Wextra
EXPORT_DYNAMIC :=
LDLIBS  :=

ifeq ($(UNAME_S),Darwin)
CC := clang
endif

ifeq ($(UNAME_S),Linux)
CC := gcc
EXPORT_DYNAMIC := -rdynamic
LDLIBS := -ldl
endif

TEST_SRC := test/main.c
TEST_TARGET := test/tests

all: test

test:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET) $(EXPORT_DYNAMIC) $(LDLIBS)
	./$(TEST_TARGET)

clean:
	rm -f $(TEST_TARGET)

.PHONY: all test clean
