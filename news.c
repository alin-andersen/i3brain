#include "news.h"

struct elist news;
pthread_t thread;
pthread_spinlock_t lock;

CURL* curl;
CURLcode res;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    //printf("%p %ld %ld %p\n", buffer, size, nmemb, userp);
}

#define NEWS_SIZE 50
#define BUFFER_SIZE 128

char* buffer;
int   buffer_len;

char* text;

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
	buffer[i] = ' ';
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

// updates the news buffer
void news_buffer_next(void)
{   
    if(beg + size != buffer_len - 1)
	beg += 1;
    else
    {
	news_buffer_init("Somebody onces told me the world is gonna roll me I ain't the sharpest tool in the shed");
	news_buffer_next();
    }

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
    news_buffer_init("no news :(");
    
    ELIST_INIT(&news);
    /*
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl)
    {
	curl_easy_setopt(curl, CURLOPT_URL, "https://newsapi.org/v1/articles");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	res = curl_easy_perform(curl);

	if(res != CURLE_OK)
	{
	    //printf("ERROR");
	    return;
	}

	curl_easy_cleanup(curl);
	}*/
}

void news_dest(void)
{
//    curl_global_cleanup();
}

int timer = -1;

void news_print(enum print_type type, int ticks)
{
    news_buffer_next();

    //printf("\n%d %d %d\n", beg, end, end-beg);
    
    blk_begin();
    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    prop_begin(PROP_COLOR);
    printf("\"%s\"", color_string(COLOR_GREEN));
    prop_end(NOT_LAST);
    prop_begin(PROP_FULL_TEXT);
    printf("\"|%s|\"", text);
    prop_end(LAST);
    blk_end(type);
}
