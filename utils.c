#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

/* func that checks if a given operand in string type is a number or not */
int isNum(char *str){
    if(str[0] == '#')
      return 1;
    return 0;
}

char * cutWhiteChars( char *old ){
    char *newstr = malloc(strlen(old)+1);
    
    char *np = newstr, *op = old;
    do {
        if (!isspace(*op))
            *np++ = *op;
    } while (*op++);
    
    return newstr;
}

commandsStruct ourCommands[] = {  

  /* how to read */
  /* name, opcode, funct, num of param */
  
  /*  2 operands*/
  {"mov", 0, 0, 2},
  {"cmp", 1, 0, 2},
  
  {"add", 2, 10, 2},
  {"sub", 2, 11, 2},

  {"lea", 4, 0, 2},

  /*  1 operand */
  {"clr", 5, 10, 1},
  {"not", 5, 11, 1},
  {"inc", 5, 12, 1},
  {"dec", 5, 13, 1},

  {"jmp", 9, 10, 1},
  {"bne", 9, 11, 1},
  {"jsr", 9, 12, 1},

  {"red", 12, 0, 1},
  {"prn", 13, 0, 1},

  /* no operands */
  {"rts", 14, 0, 0},
  {"stop", 15, 0, 0}
};

/* func to skip all white chars from a given index */
int move_to_none_white(char *line, int i){
  for(; i < strlen(line); i++){
    if(!isspace(line[i]))
      return i;
  }
  return i;
}


/* func that checks if its a comment line or just an empty line */
int skip(char line[]){
  int i = 0;
  i = move_to_none_white(line, i);

  if(line[i] == '\0' || line[i] == ';' || line[i] == '\n')
    return 1;

  return 0;
}

/* func to check if the given string is a valid int num */
int isAIntNum(char s[]){
  int i = move_to_none_white(s,0);

  if(s[i] == '-' || s[i] == '+'){
    i++;
  }

  while(s[i]!='\0'){
    if(s[i] == '-' || s[i] == '+'){
      return 0;
    }
    
    if ( !isdigit(s[i]) ){
      return 0;
    }
    i++;
  }
  return 1;
}

/* bool func that checks if the given string is one of our commands */
int isACommand(char myStr[]){
  if(!strcmp(myStr,"mov"))
    return 1;
  if(!strcmp(myStr,"cmp"))
    return 1;
  if(!strcmp(myStr,"add"))
    return 1;
  if(!strcmp(myStr,"sub"))
    return 1;
  if(!strcmp(myStr,"lea"))
    return 1;  
  if(!strcmp(myStr,"clr"))
    return 1;
  if(!strcmp(myStr,"not"))
    return 1;
  if(!strcmp(myStr,"inc"))
    return 1;
  if(!strcmp(myStr,"dec"))
    return 1;
  if(!strcmp(myStr,"jmp"))
    return 1;
  if(!strcmp(myStr,"bne"))
    return 1;  
  if(!strcmp(myStr,"jsr"))
    return 1;
  if(!strcmp(myStr,"red"))
    return 1;
  if(!strcmp(myStr,"prn"))
    return 1;
  if(!strcmp(myStr,"rts"))
    return 1;
  if(!strcmp(myStr,"stop"))
    return 1;
  return 0;
}

/* return the number of a register or -1 if its not */
int isARegister(char line[]){
  move_to_none_white(line, 0);
  if(!strcmp(line,"r0"))
    return 0;  
  if(!strcmp(line,"r1"))
    return 1;
  if(!strcmp(line,"r2"))
    return 2;  
  if(!strcmp(line,"r3"))
    return 3;
  if(!strcmp(line,"r4"))
    return 4;  
  if(!strcmp(line,"r5"))
    return 5;
  if(!strcmp(line,"r6"))
    return 6;  
  if(!strcmp(line,"r7"))
    return 7;
  if(!strcmp(line,"r8"))
    return 8;  
  if(!strcmp(line,"r9"))
    return 9;
  if(!strcmp(line,"r10"))
    return 10;  
  if(!strcmp(line,"r11"))
    return 11;
  if(!strcmp(line,"r12"))
    return 12;  
  if(!strcmp(line,"r13"))
    return 13;
  if(!strcmp(line,"r14"))
    return 14;  
  if(!strcmp(line,"r15"))
    return 15;
  
  return -1;
}

/* check if the given name is according to the rules */
int isNameOk(char line[]){
  int i = 0;

  if(!isalpha(line[i]) || isARegister(line) != -1 || isACommand(line) || strlen(line) > MAX_LABEL_LENGTH){
    return 0;
  }
  while(line[i] != '\0' ){
    if(!isalpha(line[i]) && !isdigit(line[i])){
      return 0;
    }
    i++;
  }
  return 1;
}


commandsStruct *findCommand(char * command){
  int i = 0;
  for(; i < 17; i++){
    if(!strcmp(command, ourCommands[i].commandName))
    return &ourCommands[i];
  }
  return NULL;
}

int pancakeFunc(char *lable){
  if(lable[strlen(lable)-1] == ':')
    return 1;
  return 0;
}


/* count how many words seperated with space and commas are there in a given string */
int countWords(char line []){
  int i = 0, word = 0, inWord = 0;

  /* loop till end of string */
  while(line[i] != '\0'){
    
    if(!(isspace(line[i]) || line[i] == ',') && inWord == 0){
      word++;
      inWord = 1;
    }
    if(isspace(line[i]) || line[i] == ',' ){
      inWord = 0;
    }
    i++;
  }
  return word;
}


/* delete white chars in the end of a given string */
void trimTrailing(char * str){
  int index = -1;
  int i;

  i = 0;
  while(str[i] != '\0'){

    if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n'){
      index= i;
    }

    i++;
  }
  str[index + 1] = '\0';
}


/* counting commas in a line */
int countCommas(char line []){
  int i = 0;
  int commas = 0;
  while(line[i] != '\0'){
    if (line[i] == ',')
      commas++;
    i++;
  }
  return commas;
}

/* check if the numbers of words in a given line is the equal to param */
int isCurNumOfWords(char line[], int a){
  if(countWords(line) == a)
    return 1;
  
  return 0;
}

/* check if the number of a commas in a given string is equal to a given param */
int isValidCommas(int num, char str[]){
  int i = 0;
  int count = 0;
  int inARow = 0;
  while(str[i]!='\0'){
    /* there is two commas in a row */
    if(str[i] == ',' && inARow == 1){
      return 0;
    }
    /* found a commma char and increase the counter */
    if(str[i] == ',' && inARow == 0){
      count++;
      inARow = 1;
    }
    if(str[i] != ',' && !isspace(str[i]) ){
      inARow = 0;
    }
    i++;
  }
  /* the string has same amount of commas as the given num */
  if(count == num)
    return 1;
  
  return 0;
}
