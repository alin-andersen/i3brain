CC=gcc
CFLAGS=-g -lpthread -lcurl
OBJ = i3brain.c tick_timer.c global.c battery.c time.c cpu.c ram.c io.c

CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

i3brain: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

