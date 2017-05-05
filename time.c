#include "time.h"

void time_print(enum print_type type, int ticks)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"%02d:%02d:%02d\"", tm.tm_hour, tm.tm_min, tm.tm_sec);
    prop_end(LAST);

    blk_end(type);  
}

void date_print(enum print_type type, int ticks)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"%02d.%02d.%04d\"", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    prop_end(LAST);

    blk_end(type);
}
