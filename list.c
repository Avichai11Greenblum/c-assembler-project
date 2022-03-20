#include "list.h"

/* insert node a list */
void insert(LIST *l,char input []){
  NODE *p = (NODE*)malloc(sizeof(NODE));
  char *value  = (char*)malloc(sizeof(char)*(strlen(input)+1));
  strcpy(value,input);
  p-> val = value;
  p-> mac = 0;
  p-> ext= 0; 
  p-> ent= 0;
  p-> labDec = 0;
  p-> next = l-> head;
  l-> head = p;
  return;
}

/* check if a given list has a given node */
int has(LIST *l ,char mystr[]){
  NODE *p = l->head;
  while(p != NULL){
    if(!strcmp(p->val,mystr)){
      return 1;
    }
    p = p->next;
  }
  return 0;
}

/* create new list */
LIST* newList(){
  LIST *l = (LIST*)malloc(sizeof(LIST));
  return l;
}
/* print the given list */
void printList(LIST *l){
  NODE *p = l->head;
  while(p != NULL){
    printf(" name: %s, mac: %d, ext: %d,  ent: %d, labDec: %d\n",p->val,p->mac,p->ext,p->ent,p->labDec);
    p = p->next;
  }
  printf("\n");
}

/* return a node by search his name in the list */
NODE* getNode(LIST *l, char name[]){
  NODE *p = l->head;
  while(p != NULL){
    //printf("%s\n",p->val);
    if(!strcmp(p->val,name)){
      return p;
    }
    p = p->next;
  }
  return p;
}

/* void freeList(LIST*) */
