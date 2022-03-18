#include <stdio.h>
#include <string.h>
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

int execute(char * fileName){
  char * fileNameCopy;
  char * copy2;
  FILE *filePtr;
  LIST *l;
  char * token;
  WORD *headOfFile = NULL;
  symbolLink *head;

  filePtr = fopen(fileName, "r");
  l = validNames(filePtr, fileName);
  fseek(filePtr, 0, SEEK_SET);
  
  if(!validation( filePtr , l))
    return 0;

  strcpy( copy2, fileName );
  token = strtok(copy2, ".as");
  fseek(filePtr, 0, SEEK_SET);
  preProcessing(filePtr, token);
  fclose(filePtr); 

  filePtr = fopen(strcat(token, ".ob"), "r");
  head = symboleTableCreat(filePtr);
  fseek(filePtr, 0, SEEK_SET);
  headOfFile = firstPass(filePtr, head);
  fseek(filePtr, 0, SEEK_SET);
  secondPass(filePtr, headOfFile, head, token);
  headOfFile = headOfFile->next;
  output(headOfFile);
  freeList(headOfFile);
  freeList2(head);
}

int main(int argc, char *argv[]){

  int i;
  char fileName[FILE_NAME_MAX_SIZE];

  if ( argc == 1 ){
      printf("No files were given for the program...\n");
      exit(0);
  }
  
  for( i=1; i < argc; i++){
    strcpy(fileName, argv[i]);
    if( fileEndingValid(fileName) )
      if(!execute(fileName))
        printf("because of the errorrs the file did not compile...\n");
    else
      printf("file: %s does not ends with \".as\"  ...\n", fileName);
  }
  
  return 1;
}
