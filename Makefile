CFLAGS = -g -Wall --std=c99 -O3 \
	$(shell pkg-config --cflags sdl2)

LDFLAGS = \
	$(shell pkg-config --libs sdl2)

OBJS = \
	src/main.c.o \
	src/maths.c.o \
	src/world.c.o \
	src/field.c.o \
	src/pandemic.c.o

all: pandemic

pandemic: $(OBJS)
	$(LINK.c) -o $@ $^

%.c.o: %.c
	$(COMPILE.c) -o $@ $^

clean:
	rm -f pandemic $(OBJS)
