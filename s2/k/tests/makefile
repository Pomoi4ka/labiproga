TESTS=$(wildcard *.txt)

.SUFFIXES: .txt .out

all: $(TESTS:.txt=.out)

.txt.out:
	../main $< $(<:.txt=.prot) 2>$@ 1>$@ || :

../$(ASSETS)/tests.tex: all
	for test in $(TESTS); do \
		echo -e "\\\rule{\\\columnwidth}{0.5mm}\n\nВходной файл:\n\n\\\begin{spverbatim}"; \
		cat $$test; \
		echo "\\end{spverbatim}"; \
		echo -e "\\\rule{\\\columnwidth}{0.5mm}\n\nВывод:\n\n\\\begin{spverbatim}"; \
		cat $${test/txt/out}; \
		echo "\\end{spverbatim}"; \
		echo -e "\\\rule{\\\columnwidth}{0.5mm}\n\nПротокол:\n\n\\\begin{spverbatim}"; \
		cat $${test/txt/prot}; \
		echo "\\end{spverbatim}"; \
	done > $@
