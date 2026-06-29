# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -std=c99

# SDL2 flags
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)

# SDL2_ttf flags
SDL_TTF_CFLAGS := $(shell sdl2-config --cflags) -D_REENTRANT
SDL_TTF_LDFLAGS := -lSDL2_ttf

# Executable name
TARGET = snake

# Source files
SRC = main.c texture.c monde.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) $(SDL_TTF_CFLAGS) -o $@ $^ $(SDL2_LDFLAGS) $(SDL_TTF_LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

