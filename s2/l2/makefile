CXXFLAGS = -Wall -Wextra -pedantic -std=c++98 -g

ИСХОДНИКИ = строка.cpp \
	главный.cpp \
	блок.cpp \
	символы.cpp

ОБЪЕКТЫ = $(subst .cpp,.o,$(ИСХОДНИКИ))

.SUFFIXES: .cpp .o

.PHONY: all
all: l2

l2: $(ОБЪЕКТЫ)
	$(CXX) $(CXXFLAGS) -o $@ $(ОБЪЕКТЫ)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
