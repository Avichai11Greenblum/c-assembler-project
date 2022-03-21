#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macros.h"
#include "utils.h"

/* receives a macro and pushes it to its place in the custom struct */
MACRO *pushMacro(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head){
     int i = 0;
     MACRO *link = (struct MACRO*)malloc(sizeof(struct MACRO));
     strcpy(link->macroName, macroName);
     for(; i < numOfCommands; i++){
         strcpy(link->macroCommands[i], macroCommands[i]);
     }
     link->numOfCommands = numOfCommands;
     link->next = head;
     head = link;
     return head;
 }

/* search a certain value from the macros we have */
struct MACRO* findMacro(struct MACRO *macroHead, char macroName[]) {
    struct MACRO* current = macroHead;

    if(macroHead == NULL)
        return NULL;

    while(current != NULL) {
        if(!strcmp(current->macroName, macroName))
            return current;
        else 
         current = current->next;
    }
    return NULL;
}

/* goes over the file and inserts the macros according to their names */
void preProcessing(FILE *filePtr, char *nameOfFile){
    char line[MAX_LINE_LENGTH];
    char lineCopy[MAX_LINE_LENGTH];
    char *token;
    MACRO *head = NULL;
    MACRO *macroFound;
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int linesInMacro = 0, j;
    int inMacroFlag = 0;

    FILE *objectFile;
    objectFile = fopen(strcat(nameOfFile, ".ob"), "w");

    while (fgets(line, MAX_LINE_LENGTH, filePtr)){
        strcpy(lineCopy,line);
        if(!skip(line)){
            token = strtok(lineCopy, MACRO_CUT1);
            if(inMacroFlag == 0){
                if(!strcmp(token, "macro")){
                    inMacroFlag = 1;
                    token = strtok(NULL, MACRO_CUT1);
                    strcpy(macroName ,token);
                } 
                else{
                    macroFound = findMacro(head, token);
                    if(macroFound != NULL){
                        for(j = 0; j < macroFound->numOfCommands; j++)
                            fputs(macroFound->macroCommands[j], objectFile);
                    }else
                        fputs(line, objectFile);
                }
            } 
            else {
                if (!strcmp(token, "endm")){
                    head = pushMacro(macroName, macroCommands, linesInMacro, head);
                    inMacroFlag = 0;
                    linesInMacro = 0;
                } 
                else{
                    strcpy(macroCommands[linesInMacro], line);
                    linesInMacro++;
                }
            }
        }
    } 
    fclose(objectFile);
}
