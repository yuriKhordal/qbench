.PHONY: static shared test dist clean

qbench.so: shared

static: qbench.c qbench.h
	gcc -O3 -Wall -Wextra -pedantic -c qbench.c -o qbench.o

shared: qbench.c qbench.h
	gcc -O3 -Wall -Wextra -pedantic -fPIC -c qbench.c -o qbench.o
	gcc -shared qbench.o -o qbench.so

test: qbench testbench.c
	gcc -O3 -Wall -Wextra -pedantic testbench.c qbench.o -o test
	./test

dist: qbench.so
	tar -czvf qbench.tar.gz qbench.h qbench.o qbench.so

clean:
	rm -f test
	rm -f qbench.o
	rm -f qbench.so
	rm -f qbench.tar.gz
