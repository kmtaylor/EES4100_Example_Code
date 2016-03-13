CC := gcc
CFLAGS := -Wall

OBJS := example_app.o

example_app: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
