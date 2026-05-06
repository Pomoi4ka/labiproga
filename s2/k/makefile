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

OBJS = $(SOURCES:.cpp=.o)

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.PHONY: tests
tests: main
	$(MAKE) -C tests
