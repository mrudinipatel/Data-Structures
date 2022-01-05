all: hash.o
	clang -Wall -std=c99 -pedantic hash.o -o all

hash.o: hash.c hash.h memsys.h
	clang -Wall -std=c99 -pedantic -c hash.c

clean:
	rm *.o all

