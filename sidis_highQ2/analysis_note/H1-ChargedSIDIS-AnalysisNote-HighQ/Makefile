# =============================================================================
# Makefile for the H1 charged-hadron SIDIS high-Q2 analysis note
# =============================================================================

MAIN    := main
LATEX   := pdflatex -interaction=nonstopmode -halt-on-error
BIBTEX  := bibtex

.PHONY: all clean distclean

all: $(MAIN).pdf

$(MAIN).pdf: $(MAIN).tex references.bib $(wildcard sections/*.tex)
	$(LATEX) $(MAIN)
	$(BIBTEX) $(MAIN)
	$(LATEX) $(MAIN)
	$(LATEX) $(MAIN)

clean:
	rm -f *.aux *.log *.bbl *.blg *.out *.toc *.lof *.lot \
	      *.fls *.fdb_latexmk *.synctex.gz *.nav *.snm *.vrb \
	      *.run.xml *.bcf *.xdv
	rm -f sections/*.aux

distclean: clean
	rm -f $(MAIN).pdf
