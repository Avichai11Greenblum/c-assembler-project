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
      execute(fileName);
    else
      printf("file: %s does not ends with \".as\"  ...\n", fileName);
  }
  
  return 1;
}
