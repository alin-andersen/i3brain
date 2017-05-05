#include "cpu.h"

long user_old, nice_old, system_old, idle_old, iowait_old, irq_old, softirq_old, steal_old, guest_old, guest_nice_old = 0;

void cpu_print(enum print_type type, int ticks)
{
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    FILE* file = fopen("/proc/stat", "r");
    if(file == NULL) goto cpu_print_error;
    if(fscanf(file, "cpu  %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n"
	      , &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice) != 10)
	goto cpu_print_error;
    fclose(file);

//    printf("%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
//	   user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);

    idle_old = idle_old + iowait_old;
    idle     = idle + iowait;
    
    long non_idle_old = user_old + nice_old + system_old + irq_old + softirq_old + steal_old;
    long non_idle = user + nice + irq + softirq + steal;

    long total_old = idle_old + non_idle_old;
    long total     = idle + non_idle;
    
    float cpu_usage = (float)((total - total_old) - (idle - idle_old))/((float)(total - total_old))*100.0f;

    user = user_old;
    nice = nice_old;
    system = system_old;
    idle = idle_old;
    iowait = iowait_old;
    irq = irq_old;
    softirq = softirq_old;
    steal = steal_old;
    guest = guest_old;
    guest_nice = guest_nice_old;
    
    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"CPU %06.2f%%\"", cpu_usage);
    prop_end(LAST);

    blk_end(type);

    return;
    
cpu_print_error:;

    blk_begin();

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    prop_begin(PROP_FULL_TEXT);
    printf("\"CPU ???.??%%\"");
    prop_end(LAST);

    blk_end(type);
}
