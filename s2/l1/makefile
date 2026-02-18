CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -ggdb
ifeq ($(CXX), clang++)
	CXXFLAGS+=-fsanitize=memory,undefined
endif
main: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp
