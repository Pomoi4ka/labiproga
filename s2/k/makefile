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
	pdflatex note.tex # don't look at me like that. it's latex

everything: main tests pdf

program.tex: $(SOURCES)
	for file in *.hpp *cpp; do \
		echo -e "\n$$file:\n\n\\\begin{lstlisting}[language=C++]"; \
		cat $$file; \
		echo '\end{lstlisting}'; \
	done > program.tex

tests.tex: tests
	$(MAKE) -C tests ../tests.tex
