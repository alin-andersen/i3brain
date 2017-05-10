#include "news.h"

#include "news_fetcher.h"

char* default_text = "no news";

#define NEWS_SIZE 50
#define BUFFER_SIZE 128

char* buffer = NULL;
int   buffer_len;

char* text = NULL;
int   text_len;

int size;
int beg;
int end;

// initalizes a new news buffer
void news_buffer_init(const char* text)
{
    size = NEWS_SIZE;
    beg  = 0;
    end  = size;

    // create buffer
    if(buffer != NULL) free(buffer);
    buffer_len = strlen(text)+1+2*size;
    buffer = malloc(buffer_len);

    // init buffer
    for(int i = 0; i < buffer_len-1; i++)
	buffer[i] = '_';
    buffer[buffer_len-1] = '\0';

    // copy text into buffer
    int i = 0;
    for(int i2 = size; i2 < size + strlen(text); i2++)
    {
	buffer[i2] = text[i];
	i++;
    }

//    printf("\n'%s'\n", buffer);
}

char b[2048];
struct news_fetch* fetch;

// updates the news buffer
void news_buffer_next(void(*callback)(void))
{   
    if(beg + size != buffer_len - 1)
	beg += 1;
    else
	callback();

    if(end != buffer_len - 1)
	end += 1;

    if(text != NULL)
	free(text);
    text = malloc(size + 1);
    
    // copy part of buffer into text
    int i = 0;
    for(int i2 = beg; i2 <= end; i2++)
    {
	text[i] = buffer[i2];
	i++;
    }
    text[size-1] = '\0';

    //printf("\n'%s'\n", text);
    //printf("\n%d %d %d %d %d\n", buffer_len, size, beg, end, end-beg);
}

void news_init(void)
{
    fetch = news_fetcher_fetch();
    news_buffer_init("news on the way ...");
}

void news_dest(void)
{
    
}

void news_next(void)
{
    struct news_article* article = news_fetcher_next(fetch);
    sprintf(b, "%s %s", article->title, article->summary);
    printf("%s\n", b);
    fflush(stdout);
    news_buffer_init(b);
    news_buffer_next(news_next);
}

int timer = -1;

void news_print(enum print_type type, int ticks)
{
    return;
    
    news_buffer_next(news_next);

    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"NEWS\"");
    prop_end(LAST);

    blk_end(NOT_LAST);
    
    blk_begin();
    
    prop_begin(PROP_COLOR);
    printf("\"%s\"", color_string(COLOR_GREEN));
    prop_end(NOT_LAST);

    prop_begin(PROP_FULL_TEXT);
    printf("\"%s\"", text);
    prop_end(LAST);

    blk_end(type);
}
