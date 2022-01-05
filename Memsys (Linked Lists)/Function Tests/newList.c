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

  if (new->width!=0)
  {
    printf( "Bad width" );
  }

  else if (new->head!=MEMNULL)
  {
    printf( "Bad head" );
  }

  else
  {
    printf( "ok" );
  }

  shutdown( memsys );

  return 0;
}
