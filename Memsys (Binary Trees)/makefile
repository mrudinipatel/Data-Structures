all: tree.o
	clang -Wall -std=c99 -pedantic tree.o -o all

tree.o: tree.c tree.h memsys.h
	clang -Wall -std=c99 -pedantic -c tree.c

clean:
	rm *.o all


