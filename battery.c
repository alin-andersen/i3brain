#include "battery.h"

// read current battery value
static int battery_read_current(void)
{
    int battery_current;

    battery_current = read_int("/sys/class/power_supply/BAT0/battery_now");
    if(battery_current != -1) return battery_current;

    battery_current = read_int("/sys/class/power_supply/BAT0/charge_now");
    if(battery_current != -1) return battery_current;

    battery_current = read_int("/sys/class/power_supply/BAT0/energy_now");
    if(battery_current != -1) return battery_current;
    
    return -1;
}

// read max battery value
static int battery_read_max(void)
{
    int battery_max;

    battery_max = read_int("/sys/class/power_supply/BAT0/battery_full");
    if(battery_max != -1) return battery_max;

    battery_max = read_int("/sys/class/power_supply/BAT0/charge_full");
    if(battery_max != -1) return battery_max;

    battery_max = read_int("/sys/class/power_supply/BAT0/energy_full");
    if(battery_max != -1) return battery_max;
    
    return -1;
}

int battery_timer = -1;
int anim;

void battery_print(enum print_type type, int ticks)
{
    int battery_current = battery_read_current();
    int battery_max     = battery_read_max();

    if(battery_current == -1 || battery_max == -1)
	goto battery_print_error;

    float percent = ((float)battery_current/(float)battery_max)*100.0f;

    blk_begin();
    
    if(percent < 10)
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
    printf("\"BAT %6.2f%%\"", percent);
    prop_end(LAST);
    
    blk_end(type);
    
    return;
    
battery_print_error:;

    blk_begin();
    prop_begin(PROP_FULL_TEXT);
    printf("\"");
    printf("BAT ???.??%%");
    printf("\"");
    prop_end(LAST);
    blk_end(type);  
}
