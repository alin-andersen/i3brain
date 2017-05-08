#include "global.h"

char* color_string(enum color_type type)
{
    switch(type)
    {
    case COLOR_WHITE:
	return "#ffffff";
    case COLOR_RED:
	return "#ff0000";
    case COLOR_GREEN:
	return "#00ff00";
    case COLOR_BLUE:
	return "#0000ff";
    }
}

char* prop_string(enum prop_type type)
{
    switch(type)
    {
    case PROP_FULL_TEXT:
	return "full_text";
    case PROP_SHORT_TEXT:
	return "short_text";
    case PROP_COLOR:
	return "color";
    case PROP_BACKGROUND:
	return "background";
    case PROP_BORDER:
	return "border";
    case PROP_MIN_WIDTH:
	return "min_width";
    case PROP_ALIGN:
	return "align";
    case PROP_SEPARATOR:
	return "separator";
    case PROP_SEPARATOR_BLOCK_WIDTH:
	return "separator_block_width";
    case PROP_MARKUP:
	return "markup";
    }
}

char* prop_bool(enum prop_bool bool)
{
    switch(bool)
    {
    case PROP_TRUE:
	return "true";
    case PROP_FALSE:
	return "false";
    }
}

void line_begin(void)
{
    printf("[");
}

void line_end(enum print_type type)
{
    printf("]");
    if(type != LAST) printf(",");
}

void blk_begin(void)
{
    printf("{");
}

void blk_end(enum print_type type)
{
    printf("}");
    if(type != LAST) printf(",");
}

void prop_begin(enum prop_type type)
{
    printf("\"%s\":", prop_string(type));
}

void prop_end(enum print_type type)
{
    if(type != LAST) printf(",");
}

// PROPS

void prop_full_text(enum print_type type, const char* text)
{
    prop_begin(PROP_FULL_TEXT);
    printf("\"%s\"", text);
    prop_end(type);
}

void prop_color(enum print_type type, enum color_type color)
{
    prop_begin(PROP_COLOR);
    printf("\"%s\"", color_string(color));
    prop_end(type);
}


void prop_separator(enum print_type type, enum prop_bool bool)
{
    prop_begin(PROP_SEPARATOR);
    printf("%s", prop_bool(bool));
    prop_end(type);
}

int read_int(const char* path)
{
    FILE* file = fopen(path,"r");
    if(file == NULL) return -1;

    int i;
    if(fscanf(file, "%d", &i) != 1)
	i = -1;
    fclose(file);
 
    return i;
}
