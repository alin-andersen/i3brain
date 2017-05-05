#include "tick_timer.h"

void tick_timer_init(int* timer, int ticks, int offset)
{
    *timer = ticks + offset;
}

int tick_timer_check(int timer, int ticks)
{
    return timer <= ticks;
}
