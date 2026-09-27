ASSETS = assets

.PHONY: pdf

pdf: $(ASSETS)/l1.tex report.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS)/l1.tex: l1.s
	(echo "\begin{lstlisting}"; \
	cat $<; \
	echo "\end{lstlisting}") >$@
