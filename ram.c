#include "ram.h"

void ram_print(enum print_type type, int ticks)
{
    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"RAM 1.23GB\"");
    prop_end(LAST);

    blk_end(type);
}
