CXXFLAGS = -Wall -Wextra -pedantic -std=c++98 -ggdb -fno-exceptions -march=native -O3 -flto
SOURCES = \
	main.cpp \
	string.cpp \
	stringBlock.cpp \
	fileReader.cpp \
	chars.cpp \
	file.cpp \
	solution.cpp

OBJS = $(SOURCES:.cpp=.o)

export ASSETS = assets

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.PHONY: tests
tests: main
	$(MAKE) -C tests

.PHONY: pdf
pdf: note.pdf

note.pdf: note.tex $(ASSETS)/tests.tex $(ASSETS)/program.tex $(ASSETS)/includes.png
	pdflatex note.tex
	pdflatex note.tex # don't look at me like that. it's latex

$(ASSETS)/program.tex: $(SOURCES)
	for file in *.hpp *.cpp; do \
		printf "\n$$file:\n\n\\\begin{lstlisting}[language=C++]\n"; \
		cat $$file; \
		printf "\\\end{lstlisting}\n"; \
	done > $@

$(ASSETS)/tests.tex: main
	$(MAKE) -C tests ../$@

$(ASSETS)/includes.png: $(ASSETS)/includes.dot
	dot -Tpng $< >$@

$(ASSETS)/includes.dot: $(SOURCES)
	(echo "digraph {"; \
	grep -rn "#include \"" *.hpp *.cpp | sed 's/:.:#include /" -> /; s/^/"/'; \
	echo "}") > $@
