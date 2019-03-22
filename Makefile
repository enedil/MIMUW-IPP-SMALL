#!/usr/bin/env make

CC = gcc
#CC = ../afl-2.52b/afl-gcc
CVERSION = -std=c11
CFLAGS_DEBUG = -g -DLOG_LEVEL=3\
 		 -fsanitize=shift \
		 -fsanitize=integer-divide-by-zero \
		 -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null \
		 -fsanitize=return -fsanitize=signed-integer-overflow
CFLAGS_RELEASE = -O2

CFLAGS = $(CDEBUG) $(CVERSION) -Werror -Wall -Wextra -pedantic

EXE = quantum_history

SRC = main.c utils.c parser.c history_manager.c energy_manager.c
HEADERS = utils.h parser.h history_manager.h energy_manager.h

.DEFAULT_GOAL = all

all: $(patsubst %.c, %.o, $(SRC)) $(HEADERS)
	$(CC) $(CFLAGS) $^ -o $(EXE)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: all

release: CFALGS += $(CFLAGS_RELEASE)
release: all

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: all clean debug release

clean:
	-rm *.o $(EXE)
