TESTS=$(wildcard *.txt)

.SUFFIXES: .txt .out

all: $(TESTS:.txt=.out)

.txt.out:
	../main $< $(<:.txt=.prot) 1>$@ 2>&1 || :

../$(ASSETS)/tests.tex: all
	for test in $(TESTS); do \
		printf "\\\rule{\\\columnwidth}{0.5mm}\n\nВходной файл:\n\n\\\begin{spverbatim}\n"; \
		cat $$test; \
		printf "\\\end{spverbatim}\n"; \
		printf "\\\rule{\\\columnwidth}{0.5mm}\n\nВывод:\n\n\\\begin{spverbatim}\n"; \
		cat $${test%txt}out; \
		printf "\\\end{spverbatim}\n"; \
		printf "\\\rule{\\\columnwidth}{0.5mm}\n\nПротокол:\n\n\\\begin{spverbatim}\n"; \
		cat $${test%txt}prot; \
		printf "\\\end{spverbatim}\n"; \
	done > $@
