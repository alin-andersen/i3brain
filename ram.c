#include "ram.h"

void ram_print2(long ram)
{
}

void ram_print(enum print_type type, int ticks)
{
    long  mem_total;
    long  mem_free;
    float mem_used;
    
    FILE* file = fopen("/proc/meminfo", "r");
    if(file == NULL) goto ram_print_error;
    if(fscanf(file, "%*s%ld%*s\n", &mem_total) != 1)
	goto ram_print_error;
    if(fscanf(file, "%*s%ld%*s\n", &mem_free) != 1)
	goto ram_print_error;
    fclose(file);

    mem_used = mem_total - mem_free;

    
    blk_begin();

    prop_begin(PROP_FULL_TEXT);

    if(mem_used > 1000000)
    	printf("\"RAM %5.2fGB\"", mem_used/1000000.0f);
    else
	printf("\"RAM %5.2fMB\"", mem_used/1000.0f);

    prop_end(LAST);
    blk_end(type);

    return;

ram_print_error:;

    blk_begin();
    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    prop_begin(PROP_FULL_TEXT);
    printf("\"RAM ??.??MB\"");
    prop_end(LAST);
    blk_end(type);    
}
