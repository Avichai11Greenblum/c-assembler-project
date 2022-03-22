#include "list.h"

/* func that insert a node to a given list */
void insert(LIST *li,char input []){
  char *val = (char*)malloc(sizeof(char)*(strlen(input)+1));
  NODE *base = (NODE*)malloc(sizeof(NODE));

  strcpy(val,input);
  base-> val = val;
  base-> mac = 0;
  base-> ext= 0; 
  base-> ent= 0;
  base-> labDec = 0;
  base-> next = li-> head;
  li-> head = base;
  return;
}

/* func that checks for if a list holds the wanted node */
int checkList(LIST *l ,char mystr[]){
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
    if(!strcmp(p->val,name)){
      return p;
    }
    p = p->next;
  }
  return p;
}

