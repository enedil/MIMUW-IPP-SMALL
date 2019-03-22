#!/usr/bin/env make

CC = gcc
CVERSION = -std=c11
CFLAGS_DEBUG = -g \
		 -fsanitize=shift \
		 -fsanitize=integer-divide-by-zero \
		 -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null \
		 -fsanitize=return -fsanitize=signed-integer-overflow
CFLAGS_RELEASE = -O2
CFLAGS = $(CVERSION) -Werror -Wall -Wextra -pedantic

EXE = quantum_history

SRC = main.c utils.c parser.c history_manager.c energy_manager.c
HEADERS = utils.h parser.h history_manager.h energy_manager.h

.DEFAULT_GOAL = release

all: $(patsubst %.c, %.o, $(SRC))
	$(CC) $(CFLAGS) $^ -o $(EXE)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: all

release: CFLAGS += $(CFLAGS_RELEASE)
release: all
	strip $(EXE)


compress: 
	tar -cvzf m.radwanski.395413.tar.gz $(SRC) $(HEADERS) makefile test.sh



make.deps: $(program_C_SRCS) $(SRC)
	$(CC) $(CFLAGS) -MM $(SRC) > make.deps

include make.deps

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm *.o $(EXE)
	-rm make.deps
	-rm m.radwanski.395413.tar.gz

.PHONY: all clean debug release make.deps
