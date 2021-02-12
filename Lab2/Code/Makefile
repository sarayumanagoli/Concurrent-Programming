# Makefile

all: mysort counter

mysort: mysort.cpp mergesort.cpp algorithm.cpp

	g++ -pthread -o mysort mysort.cpp mergesort.cpp algorithm.cpp
	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "To run the code, please follow the usage as mentioned below."
	@echo "To print the author's name in the sorting program,"
	@echo "Usage: ./mysort [--name]"
	@echo "To run the sorting program in normal mode,"
	@echo "Usage: ./mysort [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS] [--alg=<fj,bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>]"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

counter: counter.cpp algorithm.cpp

	g++ -pthread -o counter counter.cpp algorithm.cpp

	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "To run the code, please follow the usage as mentioned below."
	@echo "To print the author's name in the counter program,"
	@echo "Usage: ./counter [--name]"
	@echo "To run the counter program in normal mode,"
	@echo "Usage: ./counter [--name] [-t NUMTHREADS] [-i NUMITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt]"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

.PHONY: clean

clean:
	rm -rf mysort counter out.txt

