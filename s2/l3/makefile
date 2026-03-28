CXXFLAGS = -Wall -Wextra -pedantic -std=c++98 -g -fno-exceptions

SOURCES = \
	string.cpp \
	main.cpp \
	form.cpp

OBJS = $(subst .cpp,.o,$(SOURCES))

.SUFFIXES: .cpp .o

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
