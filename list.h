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
  int labDec;
  struct NODE *next;
} NODE;

typedef struct LIST{
  NODE* head;
} LIST;

void insert(LIST*,char[]);
int has(LIST*,char[]);
LIST* newList();
void printList(LIST *l);
void freeList(LIST*); /* not exist yet */

NODE* getNode(LIST *l, char name[]);
/*
int getMac(LIST *l,char name []);
int getExt(LIST *l,char name []);
int getEnt(LIST *l,char name []);
int getLabDec(LIST *l,char name []);
*/
#endif
