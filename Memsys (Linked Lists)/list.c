#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

//****NODE FUNCTIONS****
void push (struct memsys *memsys, int *node_ptr, void *src, size_t width){
   /* This function allocates width bytes and copies it from src to memsys using setval.
    * I then follow the instructions on the pdf to record the memsys address of the node into
    * the *node_ptr pointer.
   */
    struct Node mem;

    mem.data = memmalloc(memsys, width);
    
    setval(memsys, src, width, mem.data);

    mem.next = *node_ptr;

    *node_ptr = memmalloc(memsys, sizeof(struct Node)); //cannot use width bytes again here.

    setval(memsys, &mem, sizeof(mem), *node_ptr);
}

void insert (struct memsys *memsys, int *node_ptr, void *src, size_t width){
    /* I basically follow the exact instructions in the pdf. I decide to store the second 
     * memmalloc into an int variable instead of a node because I only need to store the location of memory.
    */
    struct Node old;
    struct Node new;
    int var = 0;

    getval(memsys, &old, sizeof(struct Node), *node_ptr); 

    new.data = memmalloc(memsys, width);
    setval(memsys, src, width, new.data);

    new.next = old.next;

    var = memmalloc(memsys, sizeof(struct Node));

    setval(memsys, &new, sizeof(new), var);

    old.next = var;

    setval(memsys, &old, sizeof(old), *node_ptr);
}

void delete (struct memsys *memsys, int *node_ptr){
    /* I retrieve the data for the node at *node_ptr's address in memsys using getval.
     * I then find the next node and delete it by freeing the node. This is because in this
     * assignment we are only deleting the node after the one being passed into the address.
     * I then copy the first node back into memsys using setval.
    */
    struct Node curr;
    struct Node plus;
    int temp = MEMNULL; //UPDATE: to store the 'curr' node that will be deleted

    if (memsys == NULL){
        return;
    }
    else{
        getval(memsys, &curr, sizeof(curr), *node_ptr);  
        getval(memsys, &plus, sizeof(plus), curr.next);

        temp = curr.next; //UPDATE: setting the MEMNULL to point to the 'next' address of the current node (one we wanna delete).
        curr.next = plus.next;

        memfree(memsys, plus.data);
        memfree(memsys, temp); //UPDATE: deletes the current node

        setval(memsys, &curr, sizeof(curr), *node_ptr);
    }
}

void readHead (struct memsys *memsys, int *node_ptr, void *dest, unsigned int width){
    /* If the list is empty, it prints error and exits the program, otherwise I just use
     * getval to copy width bytes from the data pointer into the dest.
    */
    struct Node mem;

    if(memsys == NULL){
        fprintf(stderr, "The list is empty. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, &mem, sizeof(mem), *node_ptr); 
        getval(memsys, dest, width, mem.data);
    }
}

void pop (struct memsys *memsys, int *node_ptr){
    /* If the list is empty, it prints error and exits the program, otherwise I just copy
     * width bytes from the data pointer into dest using getval. I then free the node
     * using memfree for both data and next attributes, for removal.
    */
    struct Node mem;
    int temp = MEMNULL; // you need this for freeing all the mallocs

    if(memsys == NULL){
        fprintf(stderr, "The list is empty. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, &mem, sizeof(mem), *node_ptr);
        temp = *node_ptr;
        *node_ptr = mem.next;
        memfree(memsys, mem.data);
        memfree(memsys, temp);
    }
}

int next (struct memsys *memsys, int *node_ptr){
    /* If the list is empty, it prints error and exits the program, otherwise I just use
     * getval to copy the *node_ptr pointer into a node and return the next structure it points to.
    */
    struct Node mem;

    if(memsys == NULL){
        fprintf(stderr, "The list is empty. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, &mem, sizeof(mem), *node_ptr);
        return mem.next;
    }
}

int isNull (struct memsys *memsys, int *node_ptr){
    // Basically just followed the instructions in the pdf.
    if(*node_ptr == MEMNULL){ //because in this case, *node_ptr points to the head of the list
        return 1;
    }
    else{
        return 0;
    }
}

//****LIST FUNCTIONS****
struct List *newList (struct memsys *memsys, unsigned int width){
    /* I use malloc to allocate memory for list. I account for the malloc failing and treat the case
     * by printing an error message and exiting the program. I then set its attributes as instructed
     * and return the struct pointer.
    */
    struct List * temp;

    temp = malloc(sizeof(struct List));

    if(temp == NULL){
        fprintf(stderr, "The malloc has failed. Please try again.\n");
        exit(0);
    }

    temp->width = width;
    temp->head = MEMNULL;

    return temp;
}

int isEmpty (struct memsys *memsys, struct List *list){
    // Basically just follow the instruction in the pdf.
    if(list->head == MEMNULL){
        return 1;
    }
    else{
        return 0;
    }
}

void freeList (struct memsys *memsys, struct List *list){
    // I follow the instructions in the pdf and free the struct pointer.
    while(list->head != MEMNULL){
        pop(memsys, &list->head);
    }

    free(list); //UPDATE: freeing the actual list structure.
}

void readItem (struct memsys *memsys, struct List *list, unsigned int index, void *dest){  
    /* I loop through the list until I reach index and retrieve data from it using next.
     * I then call insert to add the node containing the data at dest.
    */
    int val = list->head;

    for(int i = 0; i < index; i++){
        val = next(memsys, &val);
    }
    readHead(memsys, &val, dest, list->width);  
}

void appendItem (struct memsys *memsys, struct List *list, void *src){
    /* I check if the list is empty, if so, I remove the head and exit the program. Otherwise, 
     * I loop through until isNull returns true and find the next data using the next function
     * and call insert to add the node containing the data to the end of the list. I do this by
     * passing in MEMNULL in insert (which is -1) which refers to the index after everything has been deleted.
    */
    int val = list->head;
    int end = MEMNULL; // represents the end of the list
    int size = list->width;

    if(isEmpty(memsys, list)){
        push(memsys, &list->head, src, size);
        return; //can't exit here, since we need to return back to the program.
    }
    else{
        while(!isNull(memsys, &val)){
            end = val;
            val = next(memsys, &val);
        }

        insert(memsys, &end, src, size);  
    }
}

void insertItem (struct memsys *memsys, struct List *list, unsigned int index, void *src){
    /* I consider if index is 0, in which case I push the item to the head of the list. Otherwise,
     * I just loop through the list until I reach index and find the next data and insert the data
     * at that index.
    */
    int val = list->head;

    if(index == 0){
        push(memsys, &list->head, src, list->width);
    }
    else{
        for(int i = 0; i < (int)index; i++){ // UPDATE: I need to parse the unsigned int into an int to be more compatiable with the loop
            val = next(memsys, &val);
        }
        insert(memsys, &val, src, list->width);
    }
}

void prependItem (struct memsys *memsys, struct List *list, void *src){
    //I just push the data into the list's head.
    push(memsys, &list->head, src, list->width);
}

void deleteItem (struct memsys *memsys, struct List *list, unsigned int index){
    /* I consider if index is 0, in which case I remove the head. Otherwise, I just
     * loop the list until I reach index and find the next data and delete that data.
     * Again, I use MEMNULL here to reset the pointer to the end of the list.
    */
    int val = list->head;
    int end = MEMNULL; //represents the end of the list

    if(index == 0){
        pop(memsys, &list->head);
    }
    else{
        for(int i = 0; i < (int)index; i++){ // UPDATE: I need to parse the unsigned int into an int to be more compatiable with the loop
            end = val;
            val = next(memsys, &val);
        }

        delete(memsys, &val);
    }
}


