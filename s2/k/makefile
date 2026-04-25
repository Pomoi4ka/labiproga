CXXFLAGS = -Wall -Wextra -pedantic -std=c++98 -ggdb -fno-exceptions
SOURCES = \
	main.cpp \
	string.cpp \
	stringBlock.cpp \
	consts.cpp \
	fileReader.cpp \
	chars.cpp \
	file.cpp

ifeq ($(CXX),clang++)
	CXXFLAGS += -fsanitize=memory,undefined
endif

OBJS = $(SOURCES:.cpp=.o)

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
