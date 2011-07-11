targetnamea = testdesin
destdir = ../../pdfreports
all: ${targetnamea}.pdf 

$(targetnamea).pdf: $(targetnamea).tex
	xelatex $(targetnamea).tex && xelatex $(targetnamea).tex && \
	xelatex $(targetnamea).tex
	cp $@ $(destdir)



.PHONY: clean
clean:
	-rm *.toc *.aux *.brf *.out *.log *.pdf 

	

