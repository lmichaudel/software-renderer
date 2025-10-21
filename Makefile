SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2)

main: main.cpp gfx*.hpp 
	g++ -std=c++23 $(SDL_LDFLAGS) $(SDL_CFLAGS) -g main.cpp  -o main.a 

.PHONY: all 
all: main 
	./main.a