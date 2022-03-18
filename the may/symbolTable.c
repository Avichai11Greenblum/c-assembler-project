#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "firstPass.h"

symbolLink *symboleTableCreat(FILE *filePointer)
{
    int i = 0;
    char line[81];
    char *lableName;
    symbolLink *head;
    symbolLink *lableFound;
    while (fgets(line, 81, filePointer))
    {
        lableName = strtok(line, CUT);
        if(lableName[strlen(lableName)-1] == ':')
            head = addSymbol(head, lableName);
        else if(!strcmp(lableName, ".extern"))
        {
            lableName = strtok(NULL, CUT);
            head = addSymbol(head, lableName);
            head->visibility = 2;
        } else if(!strcmp(lableName, ".entry"))
        {
            lableFound = findSymbol(head, lableName);
            if(lableFound == NULL)
            {
                head = addSymbol(head, lableName);
                head->visibility = 1;
            }else
                lableFound->visibility = 1;
        }
    }
    return head;
}

symbolLink *addSymbol(symbolLink *head, char lableName[])
{
    struct symbolLink *link = (struct symbolLink *)malloc(sizeof(struct symbolLink));
    strcpy(link->name, lableName);
    link->next = head;
    head = link;
    return head;
}

struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]) {
    struct symbolLink* current = head;
    if(head == NULL)
        return NULL;
   while(current != NULL) {
        if(!strcmp(current->name, lableName))
            return current;
        else 
         current = current->next;
   }
   return NULL;
}
