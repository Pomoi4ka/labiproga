ASSETS = assets

.PHONY: pdf

pdf: report.pdf

report.pdf: $(ASSETS)/l1.tex report.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS):
	mkdir -p $(ASSETS)

$(ASSETS)/l1.tex: l1.s $(ASSETS)
	(printf "\\\begin{lstlisting}\n"; \
	cat $<; \
	printf "\\\end{lstlisting}\n") >$@
