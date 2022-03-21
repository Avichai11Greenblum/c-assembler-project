#ifndef _toBinary_H_
#define _toBinary_H_

#include "symbolTable.h"
#include "validation.h"

/*an objest that holds the 20 bytes array for each word in line and a pointer to the next (implementation of linked list)*/
typedef struct WORD{
    int word[20];
    struct WORD *next;
}WORD;

void output(WORD *head, char *fileName);
void freeList1(WORD *head);
WORD *firstPass(FILE *filePointer, symbolLink *headOfTable);
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile);
void toBinaryGuidance(char line[], WORD *headOfFile);
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable);
WORD *charToBinary(char ch);
void addWord(WORD *head, WORD *link);
int extractRegister(char * param);
int *decToBinary(int num);
void zeroMe(int word[]);
void deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile);
int allZero(WORD *link);
void changeWord(WORD *headOfFile, symbolLink *symbolFound, FILE *entFile, FILE *extFile);
void secondPass(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable, char *fileName);
void binToSpecial (int num [],int address,FILE* output);
char binToHex(int num []);

#endif
