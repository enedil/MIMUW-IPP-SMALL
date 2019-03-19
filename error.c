#include <stdio.h>
#include <stdlib.h>

void 
error()
{
    fprintf(stderr, "ERROR\n");
}


void panic()
{
    exit(EXIT_FAILURE);
}
