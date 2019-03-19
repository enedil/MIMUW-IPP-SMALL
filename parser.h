#define MAX_ARG_LIST_SIZE 2

enum operations
{
    // No operation, empty line, or comment.
    o_nop,
    o_error,
    o_declare,
    o_remove,
    o_valid,
    // Unary energy.
    o_energy1,
    // Diary energy.
    o_energy2,
    o_equal
};  

struct call_data
{
   enum operations op;
   char *args[MAX_ARG_LIST_SIZE];
};

struct call_data parse(const char*);

