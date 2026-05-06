TESTS=$(wildcard *.txt)

.SUFFIXES: .txt .out

all: $(TESTS:.txt=.out)

.txt.out:
	../main $< $(<:.txt=.prot) 2>$@ 1>$@ || :
