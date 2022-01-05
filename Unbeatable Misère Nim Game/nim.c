#include <stdio.h>
#include <stdlib.h>
#include "nim.h"

int * new_board(int board_size){
    /* I malloc an array (arr) to board_size and return the arr pointer.
     * I also error check if the malloc failed and exit the program if so.
    */
    int * arr = malloc(sizeof(int) * board_size);

    if(arr == NULL){
        fprintf(stderr, "Error, the malloc failed. Please try again.\n");
        exit(0);
    }

    return arr;
}

struct node * mk_nim_hash(int max_hash, int board_size, int *start_board){
    /* I malloc an array of max_hash size and initialize the nodes by setting the values
     * of moves, move, and nimsum to those specified in the pdf instructions. I call the hash2board
     * function with the instructed parameters. I error check if the malloc has failed.
    */
    struct node * temp = malloc(sizeof(struct node) * max_hash);

    if (temp == NULL){
        fprintf(stderr, "Error, the malloc failed. Please try again.\n");
        exit(0);
    }

    for (int i = 0; i < max_hash; i++){
        temp[i].moves = -1;
        temp[i].nimsum = -1;
        temp[i].move = NULL;
        temp[i].board = hash2board(board_size, start_board, i);
    }

    return temp;
}

void free_board(int *board){
    free(board); // I free the board
}

void free_nim_hash(int max_hash, struct node *nim_hash){
    /* Inside a loop, I free each and every array move and board before freeing the
     * nim_hash array itself outside of the loop.
    */
    for(int i = 0; i < max_hash; i++){
        free(nim_hash[i].board);
        free(nim_hash[i].move);
    }

    free(nim_hash);
}

int *board_from_argv (int board_size, char **argv){
    /* I create a new board using the new_board function from above. I then initalize the integer
     * equivalents of the argv string using atoi and store the values inside retVal indexes. I then
     * return the pointer to the retVal array. 
    */
    int *retVal = new_board(board_size);
    
    for(int i = 0; i < board_size; i++){
        retVal[i] = atoi(argv[i]);
    }

    return retVal;
}

int *copy_board (int board_size, int *board){
    /* I created a new board using the new_board function from above. I then iterated through board_size
     * number of indexes to store each index inside the duplicate array before returning a pointer to the
     * duplicate array.
    */
    int * duplicate = new_board(board_size);
    
    for(int i = 0; i < board_size; i++) {
        duplicate[i] = board[i];
    }

    return duplicate;
}

int game_over (int board_size, int *board){
    /* I use the board_size to iterate through a loop and count the number of matches in board using a 
     * counter. If the counter is equal to 1, I return 1, otherwise I return 0 as per the pdf instructions.
    */

    int counter = 0;

    for (int i = 0; i < board_size; i++){
        counter += board[i];
    }

    if (counter == 1){
        return 1;
    }
    else{
        return 0;
    }
}

void join_graph (struct node *nim_hash, int hash, int board_size, int *start_board){
    /* I check if the moves variable is -1. If it is not, I return back to the game. Otherwise I set .moves
     * equal to 0 so I can iteratively count the total number of possible moves from the given .board. I then
     * allocate .move a size using the total number of possible moves. I then use 'temp' to initalize each possible
     * move with the appropriate row and match values (i.e. using i and j inside my nested for loop). I then use
     * the temp to compute the move hash. I pass the temp pointer into my copy_board function call and store the return
     * value of copy_board inside of .hash. I then recursively call the join_graph function using .hash.
    */
    int * temp = NULL, index = 0;;

    if(nim_hash[hash].moves != -1){
        return;
    }

    nim_hash[hash].moves = 0;

    for(int i = 0; i < board_size; i++){
        nim_hash[hash].moves += nim_hash[hash].board[i]; //.moves behaves like a counter of total possible moves here.
    }

    nim_hash[hash].move = malloc(sizeof(struct move) * nim_hash[hash].moves);
    
    for(int i = 0; i < board_size; i++){
        for(int j = 1; j <= nim_hash[hash].board[i]; j++, index++){ //CHANGE: moved index counter inside the for loop condition statement.
            nim_hash[hash].move[index].row = i; //row value
            nim_hash[hash].move[index].matches = j; //match value

            temp = copy_board(board_size,nim_hash[hash].board);
            temp[i] -= j;

            nim_hash[hash].move[index].hash = board2hash(board_size, start_board, temp);

            //recursive call
            join_graph(nim_hash, nim_hash[hash].move[index].hash, board_size, start_board);
        }
    }
}





