#include "memsys.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

int main( int argc, char **argv )
{
  struct List *new;
  struct memsys *memsys;

  memsys = init( 0, 0 );

  new = newList( memsys, 0 );

  if ( isEmpty( memsys, new ) )
  {
    printf( "ok" );
  }
  else
  {
    printf( "isEmpty Error" );
  }

  freeList( memsys, new );

  shutdown( memsys );

  return 0;
}
