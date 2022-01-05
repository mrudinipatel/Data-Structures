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

  freeList( memsys, new );

  if (memsys->memctr.malloc != memsys->memctr.free)
  {
    printf( "memfree error" );
  }
  else
  {
   printf( "ok" );
  }

  shutdown( memsys );

  return 0;
}
