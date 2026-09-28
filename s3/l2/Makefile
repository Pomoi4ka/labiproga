ASSETS = assets

.PHONY: pdf

pdf: report.pdf

report.pdf: $(ASSETS)/l2.tex report.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS):
	mkdir -p $(ASSETS)

$(ASSETS)/l2.tex: l2.c $(ASSETS)
	(printf "\\\begin{lstlisting}[language=C]\n"; \
	cat $<; \
	printf "\\\end{lstlisting}\n") >$@
