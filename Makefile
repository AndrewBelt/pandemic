CFLAGS = -g -Wall --std=c99 -O3

LDFLAGS = \
	-lpng

OBJS = \
	src/main.c.o \
	src/maths.c.o \
	src/world.c.o \
	src/field.c.o \
	src/pandemic.c.o \
	src/model.c.o

all: pandemic

pandemic: $(OBJS)
	$(LINK.c) -o $@ $^

%.c.o: %.c
	$(COMPILE.c) -o $@ $^

run:
	./pandemic

clean:
	rm -f pandemic $(OBJS)
