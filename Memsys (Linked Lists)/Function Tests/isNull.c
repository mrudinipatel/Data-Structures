#include <stdio.h>
#include <string.h>

#include "list.h"
#include "memsys.h"

#define WIDTH 128

int main( int argc, char **argv )
{
  int null = MEMNULL;
  int notnull = 4;

  struct memsys *memsys;

  memsys = init( 0, 0 );

  if ( !isNull( memsys, &null ) )
  {
    printf( "Failed to detect NULL" );
    return -1;
  }

  else if ( isNull( memsys, &notnull ) )
  {
    printf( "Mistaken NULL" );
    return -1;
  }

  else
  {
    printf( "ok" );
  }

  return 0;
}


