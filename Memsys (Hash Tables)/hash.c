#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

struct HashTable *createTable (struct memsys *memsys, unsigned int capacity, unsigned int width, int (*hash)(void *, int), int (*compar)(const void *, const void *)){
    /* In this function I allocate memory for the HashTable structure using malloc. I then determine if the malloc has failed,
     * and handle the case by sending an error message to standard error stream and exiting the program. Otherwise, I set the
     * attributes of the structure as instructed in the pdf. I allocate memory for data using memmalloc and determine if it
     * has failed. If it has, I handle the case appropriately and set each element of the memsys array to be MEMNULL. I then
     * return the address of the structure (i.e. 'tempHash') back to main().
    */
    struct HashTable *tempHash = malloc(sizeof(struct HashTable));
    int temp = MEMNULL;

    if (tempHash == NULL){
        fprintf(stderr, "The malloc has failed. Please try again.\n");
        exit(0);
    }

    tempHash->nel = 0;
    tempHash->data = memmalloc(memsys, capacity * sizeof(int));
    tempHash->compar = compar;
    tempHash->capacity = capacity;
    tempHash->width = width;
    tempHash->hash = hash;

    if (tempHash->data == MEMNULL){
        fprintf(stderr, "Error, the memmalloc failed. Please try again.\n");
        exit(0);
    }

    for (int i = 0; i < capacity; i++){
        setval(memsys, &temp, sizeof(int), tempHash->data + (i * sizeof(int)));
    }

    return tempHash;
}

void addElement (struct memsys *memsys, struct HashTable *table, int addr){
    /* In this function I allocate memory for a void pointer using malloc. I then check if nel is equal to capacity.
     * If it is, I handle the case by sending an error message to the standard output stream and exiting the program.
     * Otherwise, I retrieve the data from 'addr' into the 'local' void pointer and pass 'local' into the hash function
     * to obtain the 'index'. I then iterate through the array until the max capacity is reached (end of array), and retrieve
     * data iteratively and store it inside 'address'. I then determine if 'address' is equal to MEMNULL. If it is, I use setval
     * to copy the 'addr' into the memsys array and increment nel before freeing the malloced void pointer and returning. It is
     * crucial to return here because we need to return back to the loop. If the index is equal to capacity, I set table
     * to equal 0 as instructed by the pdf. Outside the loop, I then free the void pointer.
    */
    void * local = malloc(table->width); 
    int index = 0, address = 0;

    if(table->nel == table->capacity){
        fprintf(stderr, "It has reached max capacity. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, local, table->width, addr);
        index = table->hash(local, table->capacity);

        for(int i = index; i < table->capacity; i++){
            getval(memsys, &address, sizeof(int), table->data + (i * sizeof(int)));

            if(address == MEMNULL){
                setval(memsys, &addr, sizeof(int), table->data + (i * sizeof(int)));
                table->nel++;
                free(local);
                return;
            }

            if(i == table->capacity){
                i = 0;
            }
        }
    }
    free(local);
}

int getElement (struct memsys *memsys, struct HashTable *table, void *key){
    /* In this function, I malloc a void pointer variable similarly to my addElement function. I then
     * pass key and capacity into the hash function to obtain the index. I then use an infinite loop to
     * retrieve the integer address from the memsys array stored at data location and store it inside of
     * my 'address' variable. I then check if the address is equal to MEMNULL, in which case I retrieve the
     * data at that address and store it inside the 'local' void pointer variable. I pass 'local' into the
     * compar function to obtain a return value. If the return value is 0, it means there is a match and I
     * free the 'local' variable before returning the address. Otherwise, I increment the counter to iterate
     * through the while loop and determine if the next increment of the counter is equal to the capacity. If
     * it is, I set counter to equal 0 and break out of the loop. Outside the loop, I free 'local' and return 
     * MEMNULL (because in this case, the address would be MEMNULL).
    */
    int index = 0, counter = index, address = MEMNULL, val = 0;
    void * local = malloc(table->width);

    index = table->hash(key, table->capacity);

    while(1){
        getval(memsys, &address, sizeof(int), table->data + (counter * sizeof(int)));

        if(address != MEMNULL){
            getval(memsys, local, table->width, address);

            val = table->compar(key, local);

            if(val == 0){
                free(local);
                return address;
            }
        }

        counter++;
        
        if(counter == table->capacity){
            counter = 0;
            break;
        }
    }

    free(local);
    return MEMNULL;
}

void freeTable (struct memsys *memsys, struct HashTable *table){
    //I free according to the pdf instructions - first free the data and then the structure itself.
    memfree(memsys, table->data);
    free(table);
}


