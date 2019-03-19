#!/usr/bin/env make

CC = gcc
CVERSION = -std=gnu11
CDEBUG = -g
CFLAGS = $(CDEBUG) $(CVERSION) -Werror -Wall Wextra -pedantic -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow

