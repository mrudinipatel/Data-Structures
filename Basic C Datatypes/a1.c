#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "a1.h"

/*
  Student Name: Mrudini Patel
  Student ID: 1141712
  Due Date: September 20, 2021
  Course: CIS*2520
*/

unsigned char checksum(char *string){
  /* This function checks if there is an alphabetic character in string
   * before checking if it is an uppercase or lowercase letter. If it is lowercase
   * I subtract 97 to determine it's value between 0-25 (a-z). I then add the value
   * to sum. I leave non-alphabetic characters as is. If it is uppercase I subtract 65
   * to determine its value (A-Z) and add it to the sum. I then calculate the sum modulo
   * 26 to find the checksum and return it.
  */
  int i, value, sum = 0, mod;

  for (i = 0; i < strlen(string); i++){
    if (isalpha(string[i])){
      if(islower(string[i])){
        value = string[i] - 97;

        sum += value;
      }
      else{
        value = string[i] - 65;

        sum += value;
      }
    }
    else{
      value = 0;
      sum += value;
    }
  }

  mod = sum % 26;

  return mod;
}

void caesar(char *string, int rshift){
  /* This function ensures that each letter in the string is uppercase before finding
   * its value by subtracting 65, adding the rshift, and taking the modulo of 26 before
   * adding 65 again. This allows me to apply the rshift transformation to each character in
   * the passed in string. I leave space characters as is. To deal with negative shifts, I convert
   * them first into positive shifts since a rshift of -5, for example, is the same as rshift of 21.
  */
  int i = 0;
  int value;

  if (rshift < 0){
    rshift = 26 + (rshift % 26);
  }

  for (i = 0; i < strlen(string); i++){
    if (isupper(string[i])){
      value = ((int)string[i] - 65 + rshift) % 26 + 65; // need to typecast each value
      string[i] = (char)(value);
    }
    else if (string[i] == ' '){
      // since spaces should be left as is.
    }
  }

}

void char2bits(char c, unsigned char bits[8]){
  /* This function calls the get_bits8 function 8 times to retrieve all
   * 8 bits of the char c. I to ensure I am not getting the same bit upon
   * each call, I am passing in variables that I am incrementing after each call.
   * I then store the bits in the bits array.
  */
  int i, from = 0,to = 1;

  for (i = 0; i < 8; i++){
    bits[i] = get_bits8(from, to, &c);
    from++;
    to++;
  }
}

void bits2str(int bitno, unsigned char *bits, char *bitstr){
  /* This function reads bitno numbers sotred in bits and copies each number
  * from its index in bits to bitstr. I then terminate the string by adding a
  * null character at the last index.
  */
  int i;

  for (i = 0; i < bitno; i++){
    bitstr[i] = bits[i] + 48;
    bitstr[bitno] = '\0';
  }
}

void ushort2bits(unsigned short s, unsigned char bits[16]){
  /* This function calls get_bits16 16 times. I am passing in variables
  * to prevent retrieving the same bit upon each call. I increment these
  * variables after each call. I then store the bits in the bits array.
  */
  int i, from = 0, to = 1; 

  for (i = 0; i < 16; i++){
    bits[i] = get_bits16(from, to, &s);
    from++;
    to++;
  }
}

void short2bits(short s, unsigned char bits[16]){
  /* This function call get_bits16 16 times. I am using the same logic as
   * the ushort2bits function. I am storing the bits in the bits array.
  */
  int i, from = 0, to = 1;

  for (i = 0; i < 16; i++){
    bits[i] = get_bits16(from, to, &s);
    from++;
    to++;
  }
}

short bits2short(char *bits){
  /* This function processes the bits string. I then loop through the length
   * of the bits and subtract '0' (or 48) to convert each character into an actual
   * number and store it in num. I then calculate the 2's complement of each bit by
   * multiplying the number value by 2 raised to the power of the bit's column. I
   * do this by using the left shift operator. I then add each 2's complement value
   * to the short total variable and return it.
  */
  int i, length = strlen(bits);
  short num[length];
  short value[length];
  short total = 0;

  for (i = 0; i < length; i++){
    num[i] = bits[i] - 48;
    value[i] = num[i] << (length - i - 1);
    total += value[i]; 
  }

  return total;
}
