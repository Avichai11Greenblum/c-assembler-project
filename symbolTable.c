#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbolTable.h"

/*a func to create a linked list from all the lables in the file*/
symbolLink *symboleTableCreat(FILE *filePointer){
  char line[81];
  char *token;
  symbolLink *head = (struct symbolLink*)malloc(sizeof(struct symbolLink));
  symbolLink *lableFound = NULL;
  head->next = NULL;

  while(fgets(line, 82, filePointer)){       
    token = strtok(line, CUT);
    if(token[strlen(token)-1] == ':' && findSymbol(head, strtok(token, ":")) == NULL){
      head = addSymbol(head, token);
    }
    else if(!strcmp(token, ".extern")){
      token = strtok(NULL, CUT);
      head = addSymbol(head, token);
      head->ext = 1;
      head->ent = 0;
    } 
    else if(!strcmp(token, ".entry")){          
      token = strtok(NULL, CUT);
      lableFound = findSymbol(head, token);
      if(lableFound == NULL){
          head = addSymbol(head, token);
          head->ent = 2;
      } 
      else{
        lableFound->ent = 2;
      }
    }
  }
  return head;
}

/*a func to add a lable to the linked list*/
symbolLink *addSymbol(symbolLink *head, char lableName[]){
  struct symbolLink *link = (struct symbolLink *) malloc (sizeof(struct symbolLink));
  strcpy(link->name, lableName);
  if( head == NULL ){
    link->next = NULL;
    return link;
  }
  link->ent = 1;
  link->times = 1;
  link->next = head;
  head = link;
  return head;
}

/* func that free the memory stormage from a given LIST */
void freeList(symbolLink *head){
  symbolLink *current = head;
  symbolLink *nextOne = head->next;
  while(current->next != NULL)
  {
    free(current);
    current = nextOne;
    if(current != NULL)
      nextOne = current->next;
  }
}

/*a func to find an object in the list- if found- returns the object - else - returns null*/
struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]) {
    struct symbolLink* current = head;
    if(head == NULL)
        return NULL;
    while(current != NULL)  
        if(!strcmp(current->name, lableName))
          return current;
        else
          current = current->next;
    return NULL;
}
