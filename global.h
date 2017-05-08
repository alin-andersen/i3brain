#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

enum print_type
{
    NOT_LAST,
    LAST
};

enum color_type
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
};

enum prop_type
{
    PROP_FULL_TEXT,
    PROP_SHORT_TEXT,
    PROP_COLOR,
    PROP_BACKGROUND,
    PROP_BORDER,
    PROP_MIN_WIDTH,
    PROP_ALIGN,
    PROP_SEPARATOR,
    PROP_SEPARATOR_BLOCK_WIDTH,
    PROP_MARKUP
};

enum prop_bool
{
    PROP_TRUE,
    PROP_FALSE
};

extern char* color_string(enum color_type type);
extern char* prop_string(enum prop_type type);

extern void line_begin(void);
extern void line_end(enum print_type type);

extern void blk_begin(void);
extern void blk_end(enum print_type type);

extern void prop_begin(enum prop_type type);
extern void prop_end(enum print_type type);

extern int read_int(const char* path);

#endif
