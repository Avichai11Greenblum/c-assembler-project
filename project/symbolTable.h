#include <stdio.h>

#ifndef _symbolTable_H_
#define _symbolTable_H_

#define CUT "\t ,\n"

/*an object that holds all info of a lable and a pointer to the next(implementation of linked list)*/
typedef struct symbolLink{
	char name[31];
	int adress;
	int ext; 
	int ent;
	int times;
	struct symbolLink *next;

}symbolLink;

void freeList(symbolLink *head);

symbolLink *symboleTableCreat(FILE *filePointer);
symbolLink *addSymbol(symbolLink *head, char lableName[]);
struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]);

#endif
