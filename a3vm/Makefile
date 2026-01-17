CFILES = $(shell find src/ -name "*.c")
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -pedantic -Iinc/
CC = gcc

.PHONY: all
all: $(OFILES)
	$(CC) $^ -o a3vm

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OFILES)
