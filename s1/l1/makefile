all: v1 v2 v3

v3 v2 v1: l1.cpp
	$(CXX) -std=c++98 -DV=$(subst v,,$@) -o $@ $< -Wall -Wextra
