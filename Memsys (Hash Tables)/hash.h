#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "memsys.h"

#ifndef HASH
#define HASH

struct HashTable{
 unsigned int capacity;
 unsigned int nel;
 unsigned int width;
 int data;
 int (*hash)( void *, int );
 int (*compar)(const void *, const void *);
};

struct HashTable *createTable (struct memsys *memsys, unsigned int capacity, unsigned int width, int (*hash)(void *, int), int (*compar)(const void *, const void *));

void addElement (struct memsys *memsys, struct HashTable *table, int addr);

int getElement (struct memsys *memsys, struct HashTable *table, void *key);

void freeTable (struct memsys *memsys, struct HashTable *table);

#endif // HASH


