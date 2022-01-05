#include <stdio.h>
#include <stdlib.h>
#include "array.h" 

struct Array * newArray(struct memsys *memsys, unsigned int width, unsigned int capacity){
  /* This function allocates memory to the Array struct using malloc and
   * sets all of the struct's attributes to their appropriate values as instructed.
   * This function also checks if the 'bytes' being passed into the memmalloc function
   * are negative because if this is the case, an error will occur. Hence the printf
   * error message to standard output and exiting the program. I then return the allocated struct Array.
  */
  struct Array *array_str;

  array_str = malloc(sizeof(struct Array));

  if ((width * capacity) < 0){
    printf("Error, the memmalloc function failed. Please try again.\n");
    exit(0);
  }

  array_str->data = memmalloc(memsys, width * capacity);

  array_str->width = width;
  array_str->capacity = capacity;
  array_str->nel = 0;

  return array_str;
}

void readItem(struct memsys *memsys, struct Array *array, unsigned int index, void *dest){
  /* This function determines whether the index is greater than or equal to the number of elements
   * stored in array, and prints out an error message and exits. Otherwise, it uses the getVal function
   * from memsys.c.
  */
  if (index >= array->nel){
    printf("Error. Please try again.\n");
    return;
  }
  else{
    getval(memsys, dest, array->width, array->data + (index * array->width));
  }
}

void writeItem(struct memsys *memsys, struct Array *array, unsigned int index, void *src){
  /* If the index is greater than or exceeds or equals to array->capacity, then the function prints an error
   * message to standard output. Otherwise it copies array->width bytes using setVal as instructed.
   * If the index is equal to the number of elements in the array, array->nel is incremented.
  */
  if ((index > array->nel) || (index >= array->capacity)){
    printf("Error, the array is full. Please try again.\n");
    exit(0);
  }
  else{
    setval(memsys, src, array->width, array->data + (index * array->width));

    if(index == array->nel){
      array->nel++;
    }
  }
}

void contract(struct memsys *memsys, struct Array *array){
  /* This function prints error message to standard output if array->nel is equal to 0. Otherwise,
   * it decremented array->nel by 1 each time.
  */
  if (array->nel == 0){
    printf("Error. Please try again.\n");
    exit(0);
  }
  else{
    array->nel--;
  }
}

void freeArray(struct memsys *memsys, struct Array *array){
  // Calls the memfree function to free the array struct.
  memfree(memsys, array->data);
  free(array); // Resubmit - added this to free the array structure.
}

void appendItem(struct memsys *memsys, struct Array *array, void *src){
  // Adds elements to the end of the array using writeItem function call.
  writeItem(memsys, array, array->nel, src);
}

void insertItem(struct memsys *memsys, struct Array *array, unsigned int index, void *src){
  /* This function iterates through a for loop that starts at the end of the array and traverses
   * until it reaches index. It takes all those elements and shifts them 1 position forward. It does
   * this by adding a space (as passed in by void*temp). It them writes src into the index of choice.
  */
  void * temp = malloc(sizeof(array->width)); //Resubmit - malloced to allocate size of array->width bytes.

  for(int i = array->nel; i > index; i--){ // Resubmit - after testing, I had to change i >= index to be just i > index because it was accounting for more indexes than required.
    readItem(memsys, array, i - 1, temp); //Resubmit - since it is no longer an address, I had to get rid of the & symbol infront of temp.
    writeItem(memsys, array, i, temp); //Resubmit - since it is no longer an address, I had to get rid of the & symbol infront of temp.
  }

  writeItem(memsys, array, index, src);
  free(temp); //Resubmit - to free the malloc I made.
}

void prependItem(struct memsys *memsys, struct Array *array, void *src){
  // This function uses insertItem, but in order to prepend, index is now needs to be 0.
  insertItem(memsys, array, 0, src); //Resubmit - since the pdf asked to use an insertItem() call, I changed this to just be a function call.
}

void deleteItem(struct memsys *memsys, struct Array *array, unsigned int index){
  /* This function iterates from index to the end of the array and creates a duplicate entry
   * which is then deleted by the 'contract' funciton. It is essentially the 'reverse' of insertItem.
  */
  void * temp;

  for (int i = index + 1; i < array->nel; i++){
    readItem(memsys, array, i, &temp);
    writeItem(memsys, array, i - 1, &temp);
  }

  contract(memsys, array);
}
