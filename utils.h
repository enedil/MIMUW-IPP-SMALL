#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#include <stdio.h>


#define QUANTUM_STATE_COUNT 4

int char_to_int(char);

// Exits the process, called if memory allocation failed.
void panic(void);

// Prints "ERROR" to stderr.
void err(void);

// Prints "OK" to stdout.
void ok(void);

// Safe average of numbers, does not overflow, rounding to zero.
uint64_t average(uint64_t, uint64_t);

// Convert string representing unsigned long long into integral value.
uint64_t atoull(const char*);

#endif
