

#ifndef _preProcess_H_
#define _preProcess_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t\n "
#define MAX_LINE_LENGTH 81
#define MAX_MACRO_COMMANDS 6

typedef struct MACRO{
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;

struct MACRO *head = NULL;


void preProcessing(FILE *fileName, char *nameOfFile);
struct MACRO* find(struct MACRO *head, char macroName[]);
struct MACRO *push(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head);


#endif
