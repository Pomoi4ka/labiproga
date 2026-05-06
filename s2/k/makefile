CXXFLAGS = -Wall -Wextra -pedantic -std=c++98 -ggdb -fno-exceptions -march=native -O3 -flto
SOURCES = \
	main.cpp \
	string.cpp \
	stringBlock.cpp \
	consts.cpp \
	fileReader.cpp \
	chars.cpp \
	file.cpp \
	solution.cpp

.SUFFIXES: .txt .out

OBJS = $(SOURCES:.cpp=.o)

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

TESTS=$(wildcard tests/*.txt)

.PHONY: tests runTests
tests: main runTests

runTests: $(TESTS:.txt=.out)

.txt.out:
	./main $< $(<:.txt=.prot) > $@ || :
