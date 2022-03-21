#ifndef _utils_H_
#define _utils_H_
#include <stdio.h>
#include <stdlib.h>

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81
#define MAX_COMMANDS_IN_MACRO 6

/* i think this is not belong here*/
typedef struct commandsStruct{
    char commandName[MAX_LABEL_LENGTH];
    int opcode;
    int funct;
    int numOfParam;
}commandsStruct;

int isNum(char *str);
char *cutWhiteChars(char *str);

int move_to_none_white(char line[], int i);
int skip(char line[]);
int isAIntNum(char []);
int isACommand(char []);
int isARegister(char []);
int isNameOk(char line []);
commandsStruct *findCommand(char * command);
int pancakeFunc(char *lable);
int countWords(char line []);
void trimTrailing(char * str);
int countCommas(char line []);
int isCurNumOfWords(char buffer[], int a);
int isValidCommas(int num, char str[]);

#endif
