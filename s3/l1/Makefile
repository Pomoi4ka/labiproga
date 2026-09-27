ASSETS = assets

.PHONY: pdf

pdf: $(ASSETS) $(ASSETS)/l1.tex report.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS):
	mkdir $(ASSETS)

$(ASSETS)/l1.tex: l1.s
	(echo -e "\\\begin{lstlisting}"; \
	cat $<; \
	echo -e "\\\end{lstlisting}") >$@
