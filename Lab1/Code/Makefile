# Makefile

mysort: mysort.cpp mergesort.cpp

	g++ -pthread -o mysort mysort.cpp mergesort.cpp
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "To run the code, please follow the usage as mentioned below."
	@echo "To print the author's name,"
	@echo "Usage: ./mysort [--name]"
	@echo "To run the program in normal mode,"
	@echo "Usage: ./mysort [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS] [--alg=<fjmerge,lkbucket>]"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

.PHONY: clean

clean:
	rm -rf mysort

