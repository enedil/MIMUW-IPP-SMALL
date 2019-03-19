#!/usr/bin/env make

CC = gcc
CVERSION = -std=gnu11
CDEBUG = -g
COPTIMIZATION_LEVEL = -O3
CFLAGS = $(COPTIMIZATION_LEVEL) $(CDEBUG) $(CVERSION) -Werror -Wall -Wextra -pedantic -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow

EXE = quantum_history

SRC = main.c history_manager.c

.c.o:
	$(CC) $(CFLAGS) -c $<


all: $(patsubst %.cpp, %.o, $(SRC))
	$(CC) $(CFLAGS) $< -o $(EXE)
