all: nim.o
	clang -Wall -std=c99 -pedantic nim.o -o all

nim.o: nim.c nim.h
	clang -Wall -std=c99 -pedantic -c nim.c

clean:
	rm *.o all

