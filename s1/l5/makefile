.PHONY: all input.txt
all: main gen

main: main.cpp
	$(CXX) -O0 -Wall -Wextra -pedantic -std=c++17 -ggdb -o $@ $<

gen: gen.c
	$(CC) -o $@ $<

input.txt: gen
	./gen 5 7 $(SEED) | tee input.txt

run: input.txt main
	./main
	cat output.txt
