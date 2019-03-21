#ifndef __UTILS_H
#define __UTILS_H


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


#endif
