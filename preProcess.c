
#include "preProcess.h"

MACRO *push(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head){
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

void preProcessing(FILE *fileName, char *nameOfFile){
    char line[MAX_LINE_LENGTH];
    char lineCopy[MAX_LINE_LENGTH];
    char *token;
    MACRO *head = NULL;
    MACRO *macroFound;
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int inMacro = 0;
    int j, linesInMacro = 0;

    FILE *objectFile = fopen(strcat(nameOfFile, ".ob"), "w");
    while (fgets(line, MAX_LINE_LENGTH, fileName))
    {
        strcpy(lineCopy,line);
        if(!skip(line)){
            token = strtok(lineCopy, CUT);
            if(inMacro == 0){
                if(!strcmp(token, "macro")){
                    inMacro = 1;
                    token = strtok(NULL, CUT);
                    strcpy(macroName ,token);
                } else{
                    macroFound = find(head, token);
                    if(macroFound != NULL){
                        for(j = 0; j < macroFound->numOfCommands; j++)
                            fputs(macroFound->macroCommands[j], objectFile);
                    }else
                        fputs(line, objectFile);
                }
            } else {
                if (!strcmp(token, "endm"))
                {
                    head = push(macroName, macroCommands, linesInMacro, head);
                    inMacro = 0;
                    linesInMacro = 0;
                } else{
                    strcpy(macroCommands[linesInMacro], line);
                    linesInMacro++;
                }
            }
        }
    } 
}


struct MACRO* find(struct MACRO *head, char macroName[]) {
    struct MACRO* current = head;
    if(head == NULL)
        return NULL;
   while(current != NULL) {
        if(!strcmp(current->macroName, macroName))
            return current;
        else 
         current = current->next;
   }
   return NULL;
}
