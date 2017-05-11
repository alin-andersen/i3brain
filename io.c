#include "io.h"

void io_print(enum print_type type, int ticks)
{
    blk_begin();
    prop_begin(PROP_FULL_TEXT);
    printf("\"");
    printf("IO_0_W100_R20");
    printf("\"");
    prop_end(LAST);
    blk_end(type);     
}
