targetnamea = gmsh
targetnameb = wakemecn
targetnamec = synckey
targetnamed = groupsummary

all: $(targetnamea).pdf $(targetnameb).pdf ${targetnamec}.pdf $(targetnamed).pdf

$(targetnamea).pdf: $(targetnamea).tex
	xelatex $^ && xelatex $^ && xelatex $^ && xelatex $^


$(targetnameb).pdf:$(targetnameb).tex
	xelatex $^ && xelatex $^ && xelatex $^ && xelatex $^

$(targetnamec).pdf: $(targetnamec).tex 
	xelatex $^ && xelatex $^ && xelatex $^ && xelatex $^

$(targetnamed).pdf: $(targetnamed).tex 
	xelatex $^ && xelatex $^ && xelatex $^ && xelatex $^



.PHONY: clean
clean:
	-rm *.toc *.aux *.brf *.out *.log *.pdf 

	

