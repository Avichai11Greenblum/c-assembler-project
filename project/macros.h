#ifndef _macros_H_
#define _macros_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 81
#define MAX_MACRO_COMMANDS 6
#define MACRO_CUT1 "\t\n "

typedef struct MACRO{
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;

MACRO *pushMacro(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head);
struct MACRO* findMacro(struct MACRO *head, char macroName[]);
void preProcessing(FILE *fileName, char *nameOfFile);

#endif
