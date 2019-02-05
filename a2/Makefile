LATEXMK = pdflatex
BIBTEX = bibtex

PROJECT = main

$(PROJECT).pdf : $(PROJECT).tex
	$(LATEXMK) $(PROJECT)

clean:
	rm *.log *.aux *.bbl *.blg
