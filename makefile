
CC = gcc
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)

SRC = src/main.c src/chip8.c src/display.c
TARGET = chip8

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
