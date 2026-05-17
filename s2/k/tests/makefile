TESTS=$(wildcard *.txt)

.SUFFIXES: .txt .out

all: $(TESTS:.txt=.out)

.txt.out:
	../main $< $(<:.txt=.prot) 2>$@ 1>$@ || :

clip: all
	for test in $(TESTS); do \
		echo -e "\\\rule{15cm}{0.5mm}\n\nВходной файл:\n\n\\\begin{verbatim}"; \
		cat $$test; \
		echo "\\end{verbatim}"; \
		echo -e "\\\rule{15cm}{0.5mm}\n\nВывод:\n\n\\\begin{verbatim}"; \
		cat $${test/txt/out}; \
		echo "\\end{verbatim}"; \
		echo -e "\\\rule{15cm}{0.5mm}\n\nПротокол:\n\n\\\begin{verbatim}"; \
		cat $${test/txt/prot}; \
		echo "\\end{verbatim}"; \
	done | xclip -selection clipboard
