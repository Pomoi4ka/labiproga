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

.PHONY:
pdf: note.pdf

note.pdf: note.tex tests.tex program.tex
	pdflatex note.tex

everything: main tests pdf

program.tex: $(SOURCES)
	echo *.hpp *.cpp | xargs -Ixx sh -c "echo -e '\n\nxx:\n\\\begin{lstlisting}[language=C++]'; cat xx; echo '\\end{lstlisting}'"  > program.tex

tests.tex: tests
	$(MAKE) -C tests ../tests.tex
