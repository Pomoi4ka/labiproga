CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -g -fno-exceptions

SRCS= \
	main.cpp \
	chunkForm.cpp \
	linesForm.cpp \
	chunk.cpp

OBJS = $(subst .cpp,.o,$(SRCS))

.SUFFIXES: .cpp .o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
