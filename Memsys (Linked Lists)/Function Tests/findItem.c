#include "memsys.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

int compar( const void *p1, const void *p2 )
{
  const int *i1, *i2;

  i1 = p1;
  i2 = p2;

  return *i2-*i1;
}

int main( int argc, char **argv )
{
  struct List *new;
  struct memsys *memsys;
  int x;
  int index = atoi( argv[1] );

  memsys = init( 1024, 64 );

  new = newList( memsys, sizeof(int) );

  for (x=0;x<10;x++)
    push( memsys, &(new->head), &x, sizeof(int) );


  printf( "%d", findItem( memsys, new, compar, &index ) );

  freeList( memsys, new );


  shutdown( memsys );

  return 0;
}
