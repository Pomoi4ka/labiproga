.PHONY: pdfs clean

# All .tex files up to 3 levels down
ALL_TEX   := $(shell find . -maxdepth 3 -type f -name '*.tex')

# ...but only the ones whose directory has a makefile (skip generated assets/)
TEX_FILES := $(foreach t,$(ALL_TEX),\
    $(if $(wildcard $(dir $(t))makefile $(dir $(t))Makefile),$(t)))

PDFS      := $(TEX_FILES:.tex=.pdf)

pdfs: $(PDFS)
	mkdir -p dist
	for p in $(PDFS); do \
		[ -f "$$p" ] || continue; \
		out=$$(printf '%s' "$$p" | sed 's|^\./||; s|/|.|g'); \
		cp "$$p" "dist/$$out"; \
	done

# For each src/X.tex, recurse into src/'s makefile to produce src/X.pdf.
# Pattern rules (unlike suffix rules) keep the directory prefix on %/$@/$<.
%.pdf: %.tex
	$(MAKE) -C $(dir $<) pdf
	@test -f "$@" || { echo "error: sub-make did not produce $@"; exit 1; }

clean:
	rm -rf dist
