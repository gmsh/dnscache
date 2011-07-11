targetnamea = gmsh
targetnameb = wakemecn 
targetnamec = synckey
targetnamed = groupsummary 

all: ${targetnamea}.pdf ${targetnameb}.pdf ${targetnamec}.pdf $(targetnamed).pdf

$(targetnamea).pdf: $(targetnamea).tex
	xelatex $(targetnamea).tex && xelatex $(targetnamea).tex 

$(targetnameb).pdf: $(targetnameb).tex 
	xelatex $(targetnameb).tex && xelatex $(targetnameb).tex

$(targetnamec).pdf: $(targetnamec).tex 
	xelatex $(targetnamec).tex && xelatex $(targetnamec).tex
$(targetnamed).pdf: $(targetnamed).tex 
	xelatex $(targetnamed).tex && xelatex $(targetnamed).tex



.PHONY: clean
clean:
	-rm *.toc *.aux *.brf *.out *.log *.pdf 

	

