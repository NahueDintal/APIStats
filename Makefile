CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm
TARGET = api_stats
SRCS = main.c stats.c data.c export.c probabilidad.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
