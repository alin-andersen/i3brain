CC=gcc
CFLAGS=-g
OBJ = i3brain.c tick_timer.c global.c battery.c time.c cpu.c ram.c news.c

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

i3brain: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
