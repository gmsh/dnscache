targetnamea = softwareRequirementsSpecification

all: ${targetnamea}.pdf 
$(targetnamea).pdf: $(targetnamea).tex
	xelatex $(targetnamea).tex && xelatex $(targetnamea).tex 

.PHONY: clean
clean:
	-rm *.toc *.aux *.brf *.out *.log *.pdf $(targetnamea).pdf 

