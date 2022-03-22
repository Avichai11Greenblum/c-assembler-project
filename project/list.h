#ifndef _list_H_
#define _list_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 typedef struct NODE{
  char *val;
  int mac;
  int ext;
  int ent;
  int labDec; /* did we declare in it or not -> boolean */
  struct NODE *next;
} NODE;

typedef struct LIST{
  NODE* head;
} LIST;

void insert(LIST*,char[]);
int checkList(LIST*,char[]);
LIST* newList();
void printList(LIST *l);

NODE* getNode(LIST *l, char name[]);

#endif
