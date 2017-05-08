#ifndef NEWS_FETCHER_H
#define NEWS_FETCHER_H

#include "global.h"
#include "elist.h"

#include <curl/curl.h>
#include <json.h>
#include <string.h>

struct news_article
{
    char* title;
    char* summary;

    struct elist list;
};

struct news_source
{
    char* name;
    char* id;

    struct elist articles;
    int articles_count;
    struct news_article* last;
    
    struct elist list;
};

struct news_fetch
{
    int sources_count;
    struct elist sources;
    struct news_source* last;
    
    size_t data_size;
    char* data;
};

extern struct news_fetch*   news_fetcher_fetch(void);
extern struct news_article* news_fetcher_next (struct news_fetch* fetch);

#endif
