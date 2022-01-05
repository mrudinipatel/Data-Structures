#include <stdio.h>
#include <string.h>

#include "list.h"
#include "memsys.h"

#define WIDTH 128

int main( int argc, char **argv )
{
  struct memsys *memsys;
  int addr = MEMNULL;
  struct Node node;
  int width;
  int i=42, j=1;
  char data[WIDTH] = "Hello, world!";
  char data_out[WIDTH] = "Bad string!";

  void *ptr1, *ptr2;

  if ( strcmp(argv[1],"1")==0 )
  {
    width = sizeof(int);
    ptr1 = &i;
    ptr2 = &j;
  }
  else // if ( strcmp(argv[1],"2")==0 )
  {
    width = WIDTH;
    ptr1 = data;
    ptr2 = data_out;
  }

  memsys = init(512,16);

  push( memsys, &addr, ptr1, width );

  if (memsys->mallocs!=2)
  {
    printf( "Wrong number of mallocs" );
    return -1;
  }

  getval( memsys, &node, sizeof(struct Node), addr );

  if ( node.next != MEMNULL )
  {
    printf( "next != MEMNULL" );
    return -1;
  }

  getval( memsys, ptr2, width, node.data );


  if ( strcmp(argv[1],"1")==0 )
  {
    if (i!=j)
    {
      printf( "i!=j %d %d", i, j );
      return -1;
    }
  }
  else if ( strcmp( argv[1],"2")==0 )
  {
    if ( strcmp(data,data_out) )
    {
      printf( "data != data_out" );
      return -1;
    }
  }

  printf( "ok" );

  shutdown( memsys );
  return 0;
}


