#include "memsys.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

int main( int argc, char **argv )
{
  struct List *new;
  struct memsys *memsys;
  int x,y,i;
  int appends = atoi( argv[1] );

  memsys = init( 1024, 64 );

  new = newList( memsys, sizeof(int) );

  for (x=0;x<10;x++)
    push( memsys, &(new->head), &x, sizeof(int) );

  for (x=10, i=0; i<appends; i++, x++)
    appendItem( memsys, new, &x );


  for (x=0;x<10+appends;x++)
  {
    readItem( memsys, new, x, &y );
    printf("%d,",y);
  }
  freeList( memsys, new );


  shutdown( memsys );

  return 0;
}

