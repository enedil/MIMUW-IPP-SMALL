#include <stdio.h>
#include <stdlib.h>


//#include "history_manager.h"
#include "parser.h"


int main()
{
    char c[] = "DECLARE history fhdj";
    struct call_data cd = parse(c);
    printf("%u", cd.op);
}
