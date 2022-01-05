#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

//****NODE FUNCTIONS****
void attachNode(struct memsys *memsys, int *node_ptr, void *src, unsigned int width){
    /* This function memmallocs a new struct Node using width bytes of data and save
     * the address into node.data. I then copy to node.data from src parameter. Then I
     * follow the pdf and set the lt and gte addresses to MEMNULL and allocate memory
     * using memmalloc inside *node_ptr. I then copy to *node_ptr from the struct node.
     * After each memmalloc I ensure to check that the memmalloc did not fail. If it did,
     * I print an error message to the standard error stream and exit the program.
    */
    struct Node node;

    node.data = memmalloc(memsys, width);

    if(node.data == MEMNULL){
        fprintf(stderr, "Error, the memmalloc failed. Please try again.\n");
        exit(0);
    }

    setval(memsys, src, width, node.data);

    node.lt = MEMNULL;
    node.gte = MEMNULL;

    *node_ptr = memmalloc(memsys, sizeof(struct Node));

    if(*node_ptr == MEMNULL){
        fprintf(stderr, "Error, the memmalloc failed. Please try again.\n");
        exit(0);
    }

    setval(memsys, &node, sizeof(node), *node_ptr);
}

void attachChild(struct memsys *memsys, int *node_ptr, void *src, unsigned int width, int direction){
    /* This function retrieves the node stored at the address pointed to by *node_ptr using getval.
     * I then follow the pdf and determine if direction is less than OR greater or equal to 0 and
     * handel each case by attaching a node using attachNode and updating the gte and lt addresses.
     * I then use setval to update the original node in memsys.
    */
    struct Node node;

    getval(memsys, &node, sizeof(node), width);  
    
    if (direction >= 0){
        attachNode(memsys, &node.gte, src, width);
    }
    else if (direction < 0){
        attachNode(memsys, &node.lt, src, width);
    }

    setval(memsys, &node, sizeof(struct Node), *node_ptr); //CHANGE: 3rd parameter. It was sizeof(node) before.
}

int comparNode(struct memsys *memsys, int *node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width){
    /* This function allocated memory inside a void pointer which I use to pass inside getval to store and retrieve
     * the data stored inside *node_ptr into the node structure I create. I then pass in the 'temp' into the
     * compar function pointer along with the target and store the return value in 'val' which I then return.
     * I also free the allocated memory inside the 'temp' variable I malloced earlier.
    */
    struct Node node;
    void * temp = malloc(sizeof(struct Node));
    int val = MEMNULL; //CHANGE: initialized it to MEMNULL instead of 0.

    getval(memsys, &node, sizeof(struct Node), *node_ptr);
    getval(memsys, temp, width, node.data);

    val = compar(target, temp);

    free(temp);

    return val;
}

int next(struct memsys *memsys, int *node_ptr, int direction){
    /* This function uses getval to retrieve the retrieve the node pointed to by *node_ptr.
     * I then follow the instructions from the pdf to return the gte address if the direction is
     * greater than or equal to 0, otherwise I return the lt address.
    */
    struct Node node;

    getval(memsys, &node, sizeof(struct Node), *node_ptr);

    if (direction >= 0){
        return node.gte;
    }
    else{
        return node.lt;
    }
}

void readNode(struct memsys *memsys, int *node_ptr, void *dest, unsigned int width){
    /* This function checks if *node_ptr is equal to MEMNULL, in which case the struct is empty.
     * So I handle this case by printing out an error message to the standard error stream and exiting
     * the program. Otherwise, I call getval to store the node pointed to by *node_ptr inside 'dest'.
    */
    struct Node node;

    if (*node_ptr == MEMNULL){
        fprintf(stderr, "Error, the struct is empty. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, &node, sizeof(struct Node), *node_ptr);
        getval(memsys, dest, width, node.data);
    }
}

void detachNode(struct memsys *memsys, int *node_ptr){
    /* This function removes a leaf node from the tree structure. I first check if *node_ptr is equal
     * to MEMNULL and handle the case similarily as readNode. Otherwise, I retrieve the node pointed to 
     * by *node_ptr and free, both, its data and the *node_ptr itself. I then set *node_ptr to MEMNULL
     * to indicate that it is empty.
    */
   struct Node node;

   if (*node_ptr == MEMNULL){
        fprintf(stderr, "Error, the struct is empty. Please try again.\n");
        exit(0);
    }
    else{
        getval(memsys, &node, sizeof(struct Node), *node_ptr);
        memfree(memsys, node.data);
        memfree(memsys, *node_ptr);
        *node_ptr = MEMNULL;
    }
}

void freeNodes(struct memsys *memsys, int *node_ptr){
    /* This function frees all the nodes including *node_ptr. I first ensure that *node_ptr is in fact
     * not empty (otherwise I can't free anything logically speaking). I then traverse through the nodes
     * using the next function calls and store the return values inside 2 int variables initialized to 0.
     * I then use recursion and call on freeNode() to free the return values (i.e. the nodes). I then detach
     * the node_ptr from the memsys structure after freeing all nodes.
    */
    int temp = MEMNULL, val = MEMNULL;

    if(*node_ptr != MEMNULL){
        val = next(memsys, node_ptr, -1); // '-1' because we need to get the .lt child.
        freeNodes(memsys, &val);
        
        temp = next(memsys, node_ptr, 1); // '1' because we need to get the .gte child.
        freeNodes(memsys, &temp);

        detachNode(memsys, node_ptr); //we cannot pass in a pointer to an int parameter, so we drop the '*' symbol in front of *node_ptr.
    }
    else{ //CHANGE: added this else statement to handle case when *node_ptr == MEMNULL.
        return;
    }
}

//****TREE FUNCTIONS****
struct Tree *newTree(struct memsys *memsys, unsigned int width){
    /* This function allocates memory for the Tree struct. I check if the malloc fails and handle the case
     * accordingly, otherwise I set the width of the structure to equal the width being passed in and the root
     * to equal MEMNULL (as pdf instructs). I then return the tree node.
    */
    struct Tree * tree;

    tree = malloc(sizeof(struct Tree));

    if(tree == NULL){
        fprintf(stderr, "The malloc has failed. Please try again.\n");
        exit(0);
    }

    tree->width = width;
    tree->root = MEMNULL;

    return tree;
}

void freeTree(struct memsys *memsys, struct Tree *tree){
    // Simply call the freeNode() function to free the root and tree structure as pdf instructs.
    freeNodes(memsys, &tree->root);
    free(tree);
}

void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *), void *src ){
    /* This function adds an item to the tree at the given root. I check if the tree is empty using tree->root.
     * If it is, I attach a node using the attachNode() and pass in the address of the tree->root as well as the
     * size of tree->width. Otherwise, I comparNode() and traverse through the tree structure until the tree->root is not
     * empty (not equal to MEMNULL). I then use the attachChild() to add a new node as per the pdf's instructions.
    */
    int val = MEMNULL, org = MEMNULL, increment = MEMNULL; //CHANGE: initialized all 3 variables to MEMNULL
    int position = tree->root;
    
    if(tree->root == MEMNULL){
        attachNode(memsys, &tree->root, src, tree->width);
    }
    else{
        do{
            org = position; //org represents the original position which is being modified after each loop.
            val = comparNode(memsys, &(tree->root), compar, src, tree->width);
            increment = next(memsys, &(tree->root), val);
            position = increment; // set the position equal to the new node (returned from the next() function) to update the position address in the structure.
        }while(position != MEMNULL);

        attachChild(memsys, &org, src, tree->width, val);
    }
}



