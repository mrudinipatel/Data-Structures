all: list.o
	clang -Wall -std=c99 -pedantic list.o -o all

list.o: list.c list.h memsys.h
	clang -Wall -std=c99 -pedantic -c list.c

clean:
	rm *.o all

