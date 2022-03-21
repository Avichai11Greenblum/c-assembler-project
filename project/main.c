#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macros.h"
#include "toBinary.h"

#define FILE_NAME_MAX_SIZE 40

/* func to check if the ending of the file is valid, returns 1 if it's valid */
int fileEndingValid( char *fileName ){

    int i;
    for( i=0; fileName[i] != 0; i++ ){
        
        if( i != 0 && fileName[i] == '.' ){

            if( strcmp( (fileName+i), ".as") == 0 )
                return 1;
        }
    }
    return 0;
}

int execute(char * fileName)
{
  char fileNameCopy[FILE_NAME_MAX_SIZE];
  char copy2[FILE_NAME_MAX_SIZE];
  FILE *filePtr;
  LIST *l = newList();
  char * token;
  WORD *headOfFile = NULL;
  symbolLink *head;
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
  headOfFile = firstPass(filePtr, head);
  fclose(filePtr); 
  filePtr = fopen(fileNameCopy, "r");
  secondPass(filePtr, headOfFile, head, strtok(fileNameCopy, "."));
  headOfFile = headOfFile->next;
  output(headOfFile, strtok(fileNameCopy, "."));
  freeList1(headOfFile);
  freeList2(head);
  return 1;
}

int main(int argc, char *argv[])
{
  int i;
  char fileName[FILE_NAME_MAX_SIZE];


  if (argc == 1 )
  {
    printf("No files were given for the program...\n");
    exit(0);
  }
  
  for( i=1; i < argc; i++)
  {
    strcpy(fileName, argv[i]);
    if( fileEndingValid(fileName)){
      if(!execute(fileName))
        printf("file: %s has failed to compile due to errors therefor no output was created for this file...\n\n", argv[i]);
      else
        printf("compilation in file %s complete! the files were created.\n\n", argv[i]);
    }
  }
  return 1;
}
