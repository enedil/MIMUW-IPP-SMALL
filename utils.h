#ifndef __UTILS_H
#define __UTILS_H


#include <stdint.h>
#include <stdio.h>


#define QUANTUM_STATE_COUNT 4

enum log_level {
    l_none,
    l_critical,
    l_info,
    l_debug
};

void logger(const char*, int, FILE*);

int char_to_int(char);

// exits the process, called if memory allocation failed
void panic(void);

// prints "ERROR" to stderr
void err(void);

// prints "OK" to stdout
void ok(void);

// safe average of numbers, does not overflow, rounding to zero;
uint64_t average(uint64_t, uint64_t);

// convert string representing unsigned long long into integral value
uint64_t atoull(const char*);

#endif
