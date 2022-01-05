#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "get_bits.h"
#include "a1.h"

//main file for testing purposes.

int main(int argc, char **argv) {
  //Checksum function

  printf("Checksum: %d\n", checksum("Hello, world!"));

  printf("\n");

  //Caesar cipher function
  char pontus[15] = "VENI VIDI VICI";

  caesar(pontus, -5);
  printf("%s\n", pontus);

  printf("\n");

  //Char2bits function
  char *alpha = "abcde";
  unsigned char bits[8];

  for (int pos = 0; pos < 5; pos++){
    printf("%c ", alpha[pos]);
    char2bits(alpha[pos], bits);
    
    for (int bit = 0; bit < 8; bit++){
      printf("%d", bits[bit]);
    }
    printf("\n");
  }

  printf("\n");

  //Bits2str function
  char bitstr[9];

  for (int pos = 0; pos < 5; pos++){
    char2bits(alpha[pos], bits);
    bits2str(8, bits, bitstr);

    printf("%c %s\n", alpha[pos], bitstr);
  }

  printf("\n");

  //Ushort2bits function
  unsigned short numbers[9] = {0,1,2,32767,32768,32769,65533,65534,65535};
  unsigned char bitss[16];
  char bitstrr[17];

  for (int pos = 0; pos < 9; pos++){
    ushort2bits(numbers[pos], bitss);
    bits2str(16, bitss, bitstrr);

    printf("%6d %s\n", numbers[pos], bitstrr);
  }

  printf("\n");

  //Short2bits function
  short numberss[9] = {-32768,-32767,-32766,-1,0,1,32765,32766,32767};
  unsigned char bitsss[16];
  char bitstrrr[17];

  for (int pos = 0; pos < 9; pos++){
    short2bits(numberss[pos], bitsss);
    bits2str(16, bitsss, bitstrrr);

    printf("%6d %s\n", numberss[pos], bitstrrr);
  }

  printf("\n");

  //Bit2short function
  short numbersss[9] = {-32768,-32767,-32766,-1,0,1,32765,32766,32767};
  unsigned char bitssss[16];
  char bitstrrrr[17];
  short number;

  for (int pos = 0; pos < 9; pos++){
    short2bits(numbersss[pos], bitssss);
    bits2str( 16, bitssss, bitstrrrr);
    number = bits2short(bitstrrrr);

    printf("%6d %s %6d\n", numbersss[pos], bitstrrrr, number);
  }

  return 0;
}


