#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include "get_bits.h"

/*
  Student Name: Mrudini Patel
  Student ID: 1141712
  Due Date: September 20, 2021
  Course: CIS*2520
*/

unsigned char checksum( char *string );
void caesar( char *string, int rshift );
void char2bits( char c, unsigned char bits[8] );
void bits2str( int bitno, unsigned char *bits, char *bitstr );
void ushort2bits( unsigned short s, unsigned char bits[16] );
void short2bits(short s, unsigned char bits[16] );
short bits2short( char *bits );
