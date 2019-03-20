#define MAX_ARG_LIST_SIZE 2

enum operations
{
    o_nop,             // no operation: empty line, or comment
    o_error,
    o_declare,
    o_remove,
    o_valid,
    o_energy1,         // unary energy command
    o_energy2,         // dyadic energy command
    o_equal
};  

struct call_data
{
   enum operations op;
   char *args[MAX_ARG_LIST_SIZE];
};

struct call_data parse(char*);

