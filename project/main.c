#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macros.h"
#include "binary.h"

#define FILE_NAME_MAX_SIZE 40

/* func to check if the ending of the file is valid, returns 1 if it's valid */
int fileCheckEnding( char *fileName ){

    int i;
    for( i=0; fileName[i] != 0; i++ ){
        
        if( i != 0 && fileName[i] == '.' ){

            if( strcmp( (fileName+i), ".as") == 0 )
                return 1;
        }
    }
    return 0;
}

int chainFunctions(char * fileName){

  char fileNameCopy[FILE_NAME_MAX_SIZE];
  char copy2[FILE_NAME_MAX_SIZE];
  FILE *filePtr;
  LIST *l;
  char * token;
  WORD *headOfFile = NULL;
  symbolLink *head;

  printf("--- activating file %s ---\n", fileName);
  filePtr = fopen(fileName, "r");
  l = validNames(filePtr, fileName);

  fclose(filePtr); 

  filePtr = fopen(fileName, "r");
  if(!validation( filePtr , l))
    return 0;
  fclose(filePtr);
  filePtr = fopen(fileName, "r");
  strcpy(copy2, fileName);
  token = strtok(copy2, ".");
  strcpy(fileNameCopy, token);
  preProcessing(filePtr, token);
  fclose(filePtr); 
  strcat(fileNameCopy, ".ob");
  filePtr = fopen(fileNameCopy, "r");
  head = symboleTableCreat(filePtr);
  fclose(filePtr); 
  filePtr = fopen(fileNameCopy, "r");
  headOfFile = goFirstTime(filePtr, head);
  fclose(filePtr); 
  filePtr = fopen(fileNameCopy, "r");
  secondGo(filePtr, headOfFile, head, strtok(fileNameCopy, "."));
  headOfFile = headOfFile->next;
  getOutput(headOfFile, strtok(fileNameCopy, "."));

  return 1;
}

int main(int argc, char *argv[]){
  int i;
  char fileName[FILE_NAME_MAX_SIZE];


  if (argc == 1 ){
    printf("At least one file is needed in order to compile...\n");
    exit(0);
  }
  
  for( i=1; i < argc; i++){
    strcpy(fileName, argv[i]);

    if( fileCheckEnding(fileName)){

      if(!chainFunctions(fileName)){
        printf("file: %s has failed to compile due to errors therefor no output was created for this file...\n\n", argv[i]);
}
      else
        printf("compilation of file %s is complete! the required files were created.\n\n", argv[i]);
    }
  }
  return 1;
}
