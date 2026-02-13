all: vis main

vis: vis.c
	$(CC) -std=c99 -Wall -Wextra -o $@ $< -lraylib -lm

main: main.cpp
	$(CXX) -std=c++98 -Wall -Wextra -o $@ $<
