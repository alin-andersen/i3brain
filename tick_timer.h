#ifndef TICK_TIMER_H
#define TICK_TIMER_H

extern void tick_timer_init(int* timer, int ticks, int offset);
extern int  tick_timer_check(int timer, int ticks);

#endif
