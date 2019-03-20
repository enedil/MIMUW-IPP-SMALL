#!/usr/bin/env make

CC = gcc
CVERSION = -std=gnu11
CDEBUG = -g
COPTIMIZATION_LEVEL = -O0
CFLAGS = $(COPTIMIZATION_LEVEL) $(CDEBUG) $(CVERSION) -Werror -Wall -Wextra \
		 -pedantic -fsanitize=shift -fsanitize=integer-divide-by-zero \
		 -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null \
		 -fsanitize=return -fsanitize=signed-integer-overflow
DEFINES = -DLOG_LEVEL=3

EXE = quantum_history

SRC = main.c utils.c parser.c # history_manager.c

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) -c $<


all: $(patsubst %.c, %.o, $(SRC))
	$(CC) $(CFLAGS) $^ -o $(EXE)

