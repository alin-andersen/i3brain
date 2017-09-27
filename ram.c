#include "ram.h"

void ram_print(enum print_type type, int ticks)
{
    long  mem_total;
    long  mem_free;
    long  mem_available;
    long  mem_used;
    
    FILE* file = fopen("/proc/meminfo", "r");
    if(file == NULL) goto ram_print_error;
    if(fscanf(file, "%*s%ld%*s\n", &mem_total) != 1)
	goto ram_print_error;
    if(fscanf(file, "%*s%ld%*s\n", &mem_free) != 1)
	goto ram_print_error;
    if(fscanf(file, "%*s%ld%*s\n", &mem_available) != 1)
	goto ram_print_error;
    fclose(file);

    mem_used = mem_total - mem_free;

    float mem_usage_percent = ((float)mem_used / (float)mem_total) * 100.0f;
    
    blk_begin();

    if(mem_usage_percent >= 90)
    {
	prop_begin(PROP_COLOR);
	printf("\"%s\"", color_string(COLOR_RED));
	prop_end(NOT_LAST);
    }
    else
    {
	prop_begin(PROP_COLOR);
	printf("\"%s\"", color_string(COLOR_GREEN));
	prop_end(NOT_LAST);
    }
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"MEM %2.0f%%\"", mem_usage_percent);
    prop_end(LAST);

    blk_end(type);

    return;

ram_print_error:;

    blk_begin();
    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    prop_begin(PROP_FULL_TEXT);
    printf("\"MEM ???.??%%\"");
    prop_end(LAST);
    blk_end(type);
}
