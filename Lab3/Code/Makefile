# Makefile

all: mysort

mysort: mysort.cpp mergesort.cpp

	g++ -pthread -o mysort -fopenmp mysort.cpp mergesort.cpp
	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "To run the code, please follow the usage as mentioned below."
	@echo "To print the author's name in the sorting program,"
	@echo "Usage: ./mysort [--name]"
	@echo "To run the sorting program in normal mode,"
	@echo "Usage: ./mysort [sourcefile.txt] [-o outfile.txt]"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

.PHONY: clean

clean:
	rm -rf mysort

