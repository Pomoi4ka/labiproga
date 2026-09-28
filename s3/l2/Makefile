ASSETS = assets

.PHONY: pdf

pdf: s3l2_report.pdf

s3l2_report.pdf: $(ASSETS) $(ASSETS)/l2.tex report.tex
	pdflatex report.tex
	pdflatex report.tex
	mv report.pdf $@

$(ASSETS):
	mkdir $(ASSETS)

$(ASSETS)/l2.tex: l2.c
	(printf "\\\begin{lstlisting}[language=C]\n"; \
	cat $<; \
	printf "\\\end{lstlisting}\n") >$@
