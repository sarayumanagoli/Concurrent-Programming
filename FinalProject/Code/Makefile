CC = g++
CFLAGS = -I ../includes
DEPS = bst.h

all:	main.cpp	bst.cpp
	$(CC) $(CFLAGS) main.cpp bst.cpp -o binarysearchtree -lpthread

test:	unit_test.cpp	bst.cpp
	$(CC) $(CFLAGS) unit_test.cpp bst.cpp -o unittest -lpthread

clean:
	rm binarysearchtree unittest