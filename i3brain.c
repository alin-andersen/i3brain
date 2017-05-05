#include "global.h"

#include "battery.h"
#include "time.h"
#include "cpu.h"
#include "ram.h"
#include "news.h"

struct sigaction sig_handler;
int running = 1;
int ticks   = 0;

void handler(int signal)
{
    switch(signal)
    {
    case SIGSTOP:
	running = 0;
	break;
    case SIGCONT:
	break;
    }
}

/*
void print_cpu(enum print_type type)
{
    

    return;
    
print_cpu_error:;

    print_block_beg();
    print_prop_beg(FULL_TEXT);
    printf("\"CPU ???.??%%\"");
    print_prop_end(PRINT);
    print_prop_beg(SEPARATOR);
    printf("false");
    print_prop_end(PRINT_LAST);
    print_block_end(type);    
}

void print_time(enum print_type type)
{
    
    print_prop_beg(SEPARATOR);
    printf("false");
    print_prop_end(PRINT_LAST);

    print_block_end(type);
}

void print_date(enum print_type type)
{
    print_prop_end(PRINT);
    print_prop_beg(SEPARATOR);
    printf("false");
    print_prop_end(PRINT_LAST);

    print_block_end(type);
}

int read_int(const char* path)
{
    FILE* file = fopen(path, "r");
    if(file == NULL) return -1;

    int i;
    if(fscanf(file, "%d", &i) != 1) return -1;
    fclose(file);
    return i;
}
*/

int main(void)
{    
    // signal handler
    sig_handler.sa_handler = handler;
    sig_handler.sa_flags   = SA_RESTART;
    sigfillset(&sig_handler.sa_mask);
    sigaction(SIGSTOP, &sig_handler, NULL);
    sigaction(SIGCONT, &sig_handler, NULL);    

    // tick time
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 250000000;
    
    printf("{\"version\":1}");
    

    line_begin();
    while(running)
    {
	line_begin();
	
	// BEGIN

	ram_print(NOT_LAST, ticks);
	cpu_print(NOT_LAST, ticks);
	time_print(NOT_LAST, ticks);
	date_print(NOT_LAST, ticks);
	battery_print(LAST, ticks);

	// END

	line_end(NOT_LAST);
	
	fflush(stdout);
	
	ticks++;
	nanosleep(&time, NULL);
    }
    line_end(LAST);

    //
	
    return 0;
}
