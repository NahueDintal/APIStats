CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11 -D_USE_MATH_DEFINES
LDLIBS = -lm

OBJ = main.o rng.o distributions.o data_source.o stats.o commands.o cli.o

apistats: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o apistats

.PHONY: clean
