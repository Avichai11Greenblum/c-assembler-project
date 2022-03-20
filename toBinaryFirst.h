#ifndef _toBinary_H_
#define _toBinary_H_

#include "symbolTable.h"
#include "validation.h"

/*an objest that holds the 20 bytes array for each word in line and a pointer to the next (implementation of linked list)*/
typedef struct WORD
{
    int word[20];
    struct WORD *next;
}WORD;
//extern struct WORD;

void output(WORD *head);
WORD *firstPass(FILE *filePointer, symbolLink *headOfTable);
void secondPass(FILE*, WORD*, symbolLink *, char *);
void freeList1(WORD *head);
void toBinaryCommand(char [], symbolLink *, WORD *);
void toBinaryGuidance(char [], WORD *);
void deliveryForBinary(commandsStruct * ,char [], symbolLink *, WORD *);
void extraWordsToBinary(char *, WORD *, symbolLink *);
void changeWord(WORD *, symbolLink *, FILE *, FILE *);
WORD *charToBinary(char ch);
void addWord(WORD *head, WORD *link);
int extractRegister(char * param);
int *decToBinary(int num);
void zeroMe(int word[]);
int allZero(WORD *link);

void binToSpecial (int num [],int address,FILE* output);
char binToHex(int num []);


#endif
