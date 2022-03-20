#include <stdio.h>


char binToHex(int num []);

/* a func to get the num in he special base specified */
void binToSpecial (int num [],int address,FILE* output){
  int num1 [] = {0,0,0,0};
  int num2 [] = {0,0,0,0};
  int num3 [] = {0,0,0,0};
  int num4 [] = {0,0,0,0};
  int num5 [] = {0,0,0,0};
  int i,j;
  char a, b, c, d, e;
  int count = 0;
  for(i=0; i<5; i++){
    for(j=0;j<4;j++){
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
  else{
    fprintf(output, "%d\tA%c-B%c-C%c-D%c-E%c\n", address, e, d, c, b, a);
  }
}

/*a func to convert a num in binary to hex base*/
char binToHex(int num []){
  if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 0) 
    return '0';
  if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '1';
  if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return '2';
  if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return '3';
  if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return '4';
  if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return '5';
  if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return '6';
  if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return '7';
  if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 0)
    return '8';
  if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '9';
  if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return 'a';
  if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return 'b';
  if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return 'c';
  if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return 'd';
  if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return 'e';
  if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return 'f';
  return '\0';
}
