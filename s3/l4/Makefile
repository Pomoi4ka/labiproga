ASSETS = assets

.PHONY: pdf

pdf: report.pdf

report.pdf: report.tex $(ASSETS)/source.tex
	pdflatex report.tex
	pdflatex report.tex

$(ASSETS):
	mkdir -p $(ASSETS)

$(ASSETS)/%.tex: %.asm $(ASSETS)
	(printf "\\\begin{lstlisting}[language={[x86masm]Assembler}]\n"; \
	cat $<; \
	printf "\\\end{lstlisting}\n") >$@
