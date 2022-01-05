all: array.o
	clang -Wall -std=c99 -pedantic array.o -o all

array.o: array.c array.h memsys.h
	clang -Wall -std=c99 -pedantic -c array.c

clean:
	rm *.o all

