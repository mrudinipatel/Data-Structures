#include "memsys.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

int main( int argc, char **argv )
{
  struct List *new;
  struct memsys *memsys;

  memsys = init( 1024, 64 );

  new = newList( memsys, sizeof(int) );

  for (int x=0;x<10;x++)
    push( memsys, &(new->head), &x, sizeof(int) );

  int y;
  readItem( memsys, new, 4, &y ); // [0]9, [1]8, ..., [4]5

  if (y==5)
  {
    printf( "ok" );
  }
  else
  {
    printf( "readItem failure %d\n", y );
  }

  freeList( memsys, new );


  shutdown( memsys );

  return 0;
}
