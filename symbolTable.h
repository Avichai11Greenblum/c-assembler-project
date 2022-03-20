#ifndef _symbolTable_H_
#define _symbolTable_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t ,\n"
#define MAX_LABEL_LENGTH 31

/*an object that holds all info of a lable and a pointer to the next(implementation of linked list)*/
typedef struct symbolLink
{
	char name[MAX_LABEL_LENGTH];
	int adress;
	int ext; 
  int ent;
  int times;
	struct symbolLink *next;

}symbolLink;

void freeList2(symbolLink *head);

symbolLink *symboleTableCreat(FILE *filePointer);
symbolLink *addSymbol(symbolLink *head, char lableName[]);
struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]);

#endif
