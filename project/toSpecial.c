#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char binToHex(int num []);

/*a func to get the num in he special base specified*/
void binToSpecial (int num [],int address,FILE* output){
  int num1 [] = {0,0,0,0};
  int num2 [] = {0,0,0,0};
  int num3 [] = {0,0,0,0};
  int num4 [] = {0,0,0,0};
  int num5 [] = {0,0,0,0};
  int i,j;
  char a, b, c, d, e;
  int count = 0;

  for(i = 0; i < 5; i++){
    for(j = 0 ; j < 4 ; j++){
      if(i == 0)
        num1[j] = num[count];
      else if(i == 1)
        num2[j] = num[count];
      else if(i == 2)
        num3[j] = num[count];
      else if(i == 3)
        num4[j] = num[count];
      else if(i == 4)
        num5[j] = num[count];
      count++;
    }
  }

  a = binToHex(num1);
  b = binToHex(num2);
  c = binToHex(num3);
  d = binToHex(num4);
  e = binToHex(num5);
  
  if(address < 1000)
    fprintf(output, "0%d\tA%c-B%c-C%c-D%c-E%c\n", address, e, d, c, b, a);
  else
    fprintf(output, "%d\tA%c-B%c-C%c-D%c-E%c\n", address, e, d, c, b, a);
}

/*a func to convert a num in binary to hex base*/
char binToHex(int num []){
  if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 0) 
    return '0';
  else if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '1';
  else if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return '2';
  else if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return '3';
  else if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return '4';
  else if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return '5';
  else if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return '6';
  else if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return '7';
  else if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 0)
    return '8';
  else if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '9';
  else if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return 'a';
  else if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return 'b';
  else if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return 'c';
  else if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return 'd';
  else if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return 'e';
  else if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return 'f';
  return '\0';
}
