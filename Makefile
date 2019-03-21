#!/usr/bin/env make

CC = gcc
#CC = ../afl-2.52b/afl-gcc
CVERSION = -std=c11
CDEBUG = -g
COPTIMIZATION_LEVEL = -O0
LOG_LEVEL=3
CFLAGS = $(COPTIMIZATION_LEVEL) $(CDEBUG) $(CVERSION) -DLOG_LEVEL=$(LOG_LEVEL) \
		 -Werror -Wall -Wextra -pedantic -fsanitize=shift \
		 -fsanitize=integer-divide-by-zero \
		 -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null \
		 -fsanitize=return -fsanitize=signed-integer-overflow

EXE = quantum_history

SRC = main.c utils.c parser.c history_manager.c energy_manager.c
HEADERS = utils.h parser.h history_manager.h energy_manager.h

all: $(patsubst %.c, %.o, $(SRC)) $(HEADERS)
	$(CC) $(CFLAGS) $^ -o $(EXE)

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) -c $<

.PHONY: clean

clean:
	-rm *.o
	-rm $(EXE)



