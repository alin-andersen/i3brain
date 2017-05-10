#include "news_fetcher.h"

#define APIKEY "b0a9121841154ddcb02478dca49ec068"
#define ARTICLES_MAX 2
#define SOURCES_MAX  2

static size_t news_fetcher_fetch_read(void *buffer, size_t size, size_t nmemb, void *userp)
{    
    struct news_fetch* fetch = (struct news_fetch*) userp;
    
    fetch->data = realloc(fetch->data, fetch->data_size + (size * nmemb) + 1);
    memcpy(&fetch->data[fetch->data_size], buffer, size * nmemb);
    fetch->data_size += size * nmemb;
    fetch->data[fetch->data_size] = '\0';
    
    return size * nmemb;
}

// parse source from json
static struct news_source* news_fetcher_parse_source(struct json_object* json)
{
    if(!json_object_is_type(json, json_type_object)) return NULL;

    // get id and name of source
    struct news_source* source = malloc(sizeof(struct news_source));

    ELIST_INIT(&source->articles);
    source->articles_count = 0;
    
    enum json_type type;
    json_object_object_foreach(json, key, val)
    {
	if(strcmp(key, "id") == 0)
	{
	    source->id = strdup(json_object_get_string(val));
	    continue;
	}
	
	if(strcmp(key, "name") == 0)
	{
	    source->name = strdup(json_object_get_string(val));
	    continue;
	}
    }

    return source;
}

void news_fetcher_article_dest(struct news_article* article)
{
    free(article->title);
    free(article->summary);
    free(article);
}

void news_fetcher_source_dest (struct news_source*  source)
{
    free(source->id);
    free(source->name);
    free(source);
}


// parse sources from json
static void news_fetcher_parse_sources(struct news_fetch* fetch, struct json_object* json)
{
    if(!json_object_is_type(json, json_type_array)) return;

    json_object* obj;
    int len = json_object_array_length(json);

    if(len > SOURCES_MAX) len = SOURCES_MAX;
    
    for(int i = 0; i < len; i++)
    {
	obj = json_object_array_get_idx(json, i);	
	struct news_source* source = news_fetcher_parse_source(obj);
	if(source == NULL) continue;

	if(i == 0) fetch->last = source;

	//printf("%s %s\n", source->id, source->name);
	
	// add source to list
	ELIST_LINK_BEFORE(&fetch->sources, &source->list);
	fetch->sources_count++;
    }
}

// parse article and add it to source
static void news_fetcher_parse_article(struct news_source* source, struct json_object* json)
{
    if(!json_object_is_type(json, json_type_object)) return;

    struct news_article* article = malloc(sizeof(struct news_article));
    if(article == NULL) return;
    
    enum json_type type;
    json_object_object_foreach(json, key, val)
    {
	if(strcmp(key, "title") == 0)
	{	   
	    if(json_object_get_string(val) == NULL) continue;
	    article->title = strdup(json_object_get_string(val));
	    continue;
	}

	if(strcmp(key,"description") == 0)
	{
	    if(json_object_get_string(val) == NULL) continue;
	    article->summary = strdup(json_object_get_string(val));
	    continue;
	}
    }

    // add article to source
    ELIST_LINK_BEFORE(&source->articles, &article->list);
    source->articles_count++;
}

// parse articles from json
static void news_fetcher_parse_articles(struct news_source* source, struct json_object* json)
{
    if(!json_object_is_type(json, json_type_array)) return;

    json_object* obj;
    int len = json_object_array_length(json);

    if(len > ARTICLES_MAX) len = ARTICLES_MAX;
    
    for(int i = 0; i < len; i++)
    {
	obj = json_object_array_get_idx(json, i);	
        news_fetcher_parse_article(source, obj);
    }
}

// fetch data from 'newsapi.org'
static void news_fetcher_newsapi(struct news_fetch* fetch, const char* url)
{
    if(fetch->data != NULL) free(fetch->data);
    fetch->data_size = 0;
    
    CURL* curl = curl_easy_init();
    if(curl == NULL) return;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    //char* buffer = malloc(CURL_ERROR_SIZE);
    //curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, news_fetcher_fetch_read);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fetch);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}

// initalizes the news_fetcher and retuns all sources and articles
struct news_fetch* news_fetcher_fetch(void)
{
    struct news_fetch* fetch = malloc(sizeof(struct news_fetch));

    ELIST_INIT(&fetch->sources);
    fetch->sources_count = 0;

    fetch->data_size = 0;
    fetch->data = NULL;

    // change this!!
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // fetch sources
    news_fetcher_newsapi(fetch, "https://newsapi.org/v1/sources?apiKey="APIKEY);
    
    // deserialize sources

    enum json_type      type;
    struct json_object* json = json_tokener_parse(fetch->data);

    json_object_object_foreach(json, key, val)
    {
	if(strcmp(key, "sources") == 0)
	{
	    news_fetcher_parse_sources(fetch, val);
	    break;
	}
    }

    json_object_put(json);
    
    // fetch articles
    char url[1048];
    
    ELIST_FOREACH_BEG(&fetch->sources);
    {
	struct news_source* source = (struct news_source*) ELIST_FOREACH_DATA(struct news_source, list);

	sprintf(url, "https://newsapi.org/v1/articles?apiKey="APIKEY"&source=%s", source->id);
	//printf("%p\n", source);

	fetch->data_size = 0;
	fetch->data = NULL;

	news_fetcher_newsapi(fetch, url);

	// deserialize articles

	if(fetch->data != NULL)
	{
	    json = json_tokener_parse(fetch->data);
	    if(json != NULL)
	    {
		json_object_object_foreach(json, key, val)
		{
		    if(strcmp(key, "articles") == 0)
		    {
			news_fetcher_parse_articles(source, val);
			break;
		    }
		}

		json_object_put(json);
	    }
	    
	}
    }
    ELIST_FOREACH_END;
    
    return fetch;
}

// returns the next article
struct news_article* news_fetcher_next(struct news_fetch* fetch)
{
    if(ELIST_EMPTY(&fetch->sources)) return NULL;

    struct news_source* source = STRUCT_OFFSET(&ELIST_FIRST(&fetch->sources), struct news_source, list);
    
    if(ELIST_EMPTY(&source->articles))
    {
	ELIST_UNLINK(&source->list);
	news_fetcher_source_dest(source);
	return news_fetcher_next(fetch);
    }

    struct news_article* article = STRUCT_OFFSET(&ELIST_FIRST(&source->articles), struct news_article, list);
    ELIST_UNLINK(&article->list);
    return article;
}
