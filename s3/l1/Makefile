ASSETS = assets

.PHONY: pdf

pdf: $(ASSETS) $(ASSETS)/l1.tex report.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS):
	mkdir $(ASSETS)

$(ASSETS)/l1.tex: l1.s
	(printf "\\\begin{lstlisting}\n"; \
	cat $<; \
	printf "\\\end{lstlisting}\n") >$@
