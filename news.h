#ifndef NEWS_H
#define NEWS_H

#include "global.h"
#include "tick_timer.h"
#include "elist.h"

#include <string.h>
#include <pthread.h>

struct news_item
{
    char* title;
    char* text;
};

extern void news_init(void);
extern void news_dest(void);
extern void news_print(enum print_type type, int ticks);

#endif
