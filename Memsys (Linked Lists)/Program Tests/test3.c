#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* This program will conduct a bunch of tests on either an array or a list.
 */

#define    CAPACITY    100    // total capacity of the array
#define NEL         20    // number of elements of the series to include



int main( int argc, char **argv )
{
  int i;
  double number;

  struct memsys *memsys;
  
  memsys = init( 32760, 512 );

  struct List *sequence;


  sequence = newList( memsys, sizeof( double ) );;


  printf( "after new\n" );
  printops( memsys ); // make sure you are using the latest version of memsys
  printf( "\n\n" );

  // fill data structure with doubles, numbered from 0 to NEL-1
  for (i=0;i<NEL;i++)
  {
    number = i;
    appendItem( memsys, sequence, &number );
  }

  printf( "after appendItem x 20\n" );
  printops( memsys );
  printf( "\n\n" );

  printf( "Initial data:\n" );
  for (i=0;i<NEL;i++)
  {
    readItem( memsys, sequence, i, &number );
    printf( "%d %f\n", i, number );
  }

  printf( "after readItem x 20\n" );
  printops( memsys );
  printf( "\n\n" );

  // now insert the value 7.5 at index 8
  number = 7.5;
  insertItem( memsys, sequence, 8, &number );
  printf( "after insertItem =7.5 at index 8\n" );
  printops( memsys );
  printf( "\n\n" );

  for (i=0;i<NEL+1;i++)
  {
    readItem( memsys, sequence, i, &number );
    printf( "%d %f\n", i, number );
  }

  printf( "after readItem x 21\n" );
  printops( memsys );
  printf( "\n\n" );

  // now prepend the value -1.0
  number = -1.0;
  prependItem( memsys, sequence, &number );
  printf( "after prependItem\n" );
  printops( memsys );
  printf( "\n\n" );


  printf( "After prepend:\n" ); 
  for (i=0;i<NEL+2;i++)
  {
    readItem( memsys, sequence, i, &number );
    printf( "%d %f\n", i, number );
  }

  printf( "after readItem x 22\n" );
  printops( memsys );
  printf( "\n\n" );

  // delete element 16
  deleteItem( memsys, sequence, 16 );
  printf( "after deleteItem\n" );
  printops( memsys );
  printf( "\n\n" );

  printf( "After delete:\n" );
  for (i=0;i<NEL+1;i++)
  {
    readItem( memsys, sequence, i, &number );
    printf( "%d %f\n", i, number );
  }

  printf( "after readItem x 21\n" );
  printops( memsys );
  printf( "\n\n" );


  freeList( memsys, sequence );

  printf( "after free\n" );
  printops( memsys );
  printf( "\n\n" );

  shutdown( memsys );
}

