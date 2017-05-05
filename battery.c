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

    prop_begin(PROP_SEPARATOR);
    printf("false");
    prop_end(NOT_LAST);
    
    // if battery is lower than 10
    // change color to red
    if(percent < 10)
    {	
	prop_begin(PROP_COLOR);
	printf("\"%s\"", color_string(COLOR_RED));
	prop_end(NOT_LAST);
    }

    prop_begin(PROP_FULL_TEXT);
    printf("\"");
    
    if(percent < 20)
    {
	if(battery_timer == -1)
	    tick_timer_init(&battery_timer, ticks, 4);

	if(tick_timer_check(battery_timer, ticks))
	{
	    tick_timer_init(&battery_timer, ticks, 4);
	    if(anim == 0) anim = 1;
	    else anim = 0;
	}
	
	// animation
	if(anim == 0) printf(" %06.2f%%", percent);
	else          printf(" %06.2f%%", percent);
    }
    else if(percent < 40) printf("");
    else if(percent < 60) printf("");
    else if(percent < 80) printf("");
    else                  printf("");
    
    printf("\"");
    prop_end(LAST);
    
    blk_end(type);
    
    return;
    
battery_print_error:;

    blk_begin();
    prop_begin(PROP_FULL_TEXT);
    printf("\"");
    printf(" ERROR");
    printf("\"");
    prop_end(LAST);
    blk_end(type);  
}
