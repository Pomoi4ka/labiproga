CXXFLAGS = -Wall -Wno-trigraphs -Wextra -pedantic -std=c++98 -g -fno-exceptions -fvisibility=hidden

SOURCES = \
	string.cpp \
	main.cpp \
	form.cpp \
	formFileIORelationship.cpp

OBJS = $(subst .cpp,.o,$(SOURCES))

.SUFFIXES: .cpp .o

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
