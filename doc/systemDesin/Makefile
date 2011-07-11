targetnamea = softwareRequirementsSpecification
targetnameb = systemDesin
destdir = ../pdfreports
#targetnamec = 03testCaseSpecification
#targetnamed = 03prototypeSpecification
#targetnamee = 04systemArchitectureDesign
#targetnamef = 05uml
#targetnameg = detailedDesign
#targetnameh = 01developmentProcess
#targetnamei = 01plan
#targetnamej = workdivision
#targetnamej = workdivision

all: ${targetnamea}.pdf ${targetnameb}.pdf 
$(targetnamea).pdf: $(targetnamea).tex
	xelatex $(targetnamea).tex && xelatex $(targetnamea).tex && \
	cp $@ $(destdir)

$(targetnameb).pdf: $(targetnameb).tex mokuaisheji.tex tongxinxieyi.tex mm.tex\
                   shejisilu.tex jiekousheji.tex
	xelatex $(targetnameb).tex && xelatex $(targetnameb).tex && \
	cp $@ $(destdir)


.PHONY: clean
clean:
	-rm *.toc *.aux *.brf *.out *.log *.pdf $(destdir)/$(targetnamea).pdf \
	$(destdir)/$(targetnameb).pdf
