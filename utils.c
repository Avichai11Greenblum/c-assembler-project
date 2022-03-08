#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

commandsStruct ourCommands[] = {
    /* first group -> have 2 operands*/
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    
    {"add", 2, 10, 2},
    {"sub", 2, 11, 2},

    {"lea", 4, 0, 2},

    /* second group -> have 1 operand */
    {"clr", 5, 10, 1},
    {"not", 5, 11, 1},
    {"inc", 5, 12, 1},
    {"dec", 5, 13, 1},

    {"jmp", 9, 10, 1},
    {"bne", 9, 11, 1},
    {"jsr", 9, 12, 1},

    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},

    /* third group -> no operands */
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}
};

/* func to skip all white chars */
int move_to_none_white(char *line, int i)
{
    for(; i < strlen(line); i++)
    {
        if(!isspace(line[i]))
            return i;
    }
    return i;
}


int skip(char line[])
{
    int i = 0;
    i = move_to_none_white(line, i);
    if(line[i] == ';' || line[i] == '\n')
        return 1;
    return 0;
}

MACRO *push(char *macroName, char *macroCommands[], int numOfCommands){
    int i = 0;
    struct MACRO *link = (struct MACRO*) malloc(sizeof(struct MACRO));
    strcpy(link->macroName, macroName);
    for(; i < 6; i++){
        strcpy(link->macroCommands[i], macroCommands[i]);
    }
    link->numOfCommands = numOfCommands;
    link->next = head;
    head = link;
    return head;
}

int isANaturalNum(char c){
  if(c <= '9' && c>= '0'){
    return 1;
  }
  return 0;
}

int isACommand(char line []){
  if(!strcmp(line,"mov"))
    return 1;
  if(!strcmp(line,"cmp"))
    return 1;
  if(!strcmp(line,"add"))
    return 1;
  if(!strcmp(line,"sub"))
    return 1;
  if(!strcmp(line,"lea"))
    return 1;  
  if(!strcmp(line,"clr"))
    return 1;
  if(!strcmp(line,"not"))
    return 1;
  if(!strcmp(line,"inc"))
    return 1;
  if(!strcmp(line,"dec"))
    return 1;
  if(!strcmp(line,"jmp"))
    return 1;
  if(!strcmp(line,"bne"))
    return 1;  
  if(!strcmp(line,"jsr"))
    return 1;
  if(!strcmp(line,"red"))
    return 1;
  if(!strcmp(line,"prn"))
    return 1;
  if(!strcmp(line,"rts"))
    return 1;
  if(!strcmp(line,"stop"))
    return 1;
  return 0;
}

int isARegister(char line []){
  if(!strcmp(line,"r0"))
    return 1;  
  if(!strcmp(line,"r1"))
    return 1;
  if(!strcmp(line,"r2"))
    return 1;  
  if(!strcmp(line,"r3"))
    return 1;
  if(!strcmp(line,"r4"))
    return 1;  
  if(!strcmp(line,"r5"))
    return 1;
  if(!strcmp(line,"r6"))
    return 1;  
  if(!strcmp(line,"r7"))
    return 1;
  if(!strcmp(line,"r8"))
    return 1;  
  if(!strcmp(line,"r9"))
    return 1;
  if(!strcmp(line,"r10"))
    return 1;  
  if(!strcmp(line,"r11"))
    return 1;
  if(!strcmp(line,"r12"))
    return 1;  
  if(!strcmp(line,"r13"))
    return 1;
  if(!strcmp(line,"r14"))
    return 1;  
  if(!strcmp(line,"r15"))
    return 1;
  return 0;
}

int isNameOk(char line []){
  int i = 0;
  if(!isalpha(line[i]) || isARegister(line) || isACommand(line)){
    return 0;
  }
  while(line[i] != '\0'){
    if(!isalpha(line[i]) && !isANaturalNum(line[i])){
      return 0;
    }
    i++;
  }
  return 1;
}

commandsStruct *findCommand(char * command)
{
    int i = 0;
    for(; i < 17; i++)
    {
        if(!strcmp(command, ourCommands[i]->commandName))
        return ourCommands[i];
    }
    return NULL;
}

int isLableDec(char *lable)
{
    if(lable[strlen(lable)-1] == ':')
        return 1;
    return 0;
}

int isANum(char *param)

{
    if(param[0] == '#')
        return 1;
    return 0;
}
