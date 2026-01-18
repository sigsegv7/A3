CFILES = $(shell find . -name "*.c")
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -pedantic -Iinc
CC = gcc

.PHONY: all
all: $(OFILES)
	$(CC) $^ -o as3

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
