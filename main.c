#include <stdio.h>
#include <stdlib.h>


#include "history_manager.h"
#include "parser.h"


int main()
{
    enum operations c = o_nop;
    printf("%d", c);
    parse("xdr");
}
