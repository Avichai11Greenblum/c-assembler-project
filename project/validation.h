#ifndef _validation_H_
#define _validation_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "list.h"

#define parse " \t\n"
#define parse_words "  \t\n"
#define parse_delivery " ,\t\n["
#define parse_commands " ,\t\n:"

int validation(FILE *filePtr, LIST *names);
int isRight(char line [] ,LIST *names);

LIST *validNames(FILE *fileName, char *nameOfFile);
int whichDelivery(char myStr[], LIST *names);

int validOperands(char [], char [], char [], LIST*);

void insertMa(char [], LIST*);
void insertEx(char [], LIST*);
void insertEn(char [], LIST*);
void insertLD(char [], LIST*);

int checkForMacroAtSecond( LIST *names, char token[], int lineNumber );
int checkForExternAtSecond( LIST *names, char token[], int lineNumber );
int checkForEntryAtSecond( LIST *names, char token[], int lineNumber );
int checkForLabelAtBegining( LIST *names, char token[], int lineNumber);

#endif
