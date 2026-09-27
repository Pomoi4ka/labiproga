.PHONY: pfds

TEX_FILES != find -maxdepth 3 -name '*.tex'
PDFS = $(subst .tex,.pdf,$(TEX_FILES))

.SUFFIXES: .tex .pdf

pdfs: $(PDFS)
	mkdir -p dist
	cp $(PDFS) dist

.tex.pdf:
	make -C $(dir $<) pdf
