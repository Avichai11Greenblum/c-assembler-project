#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "binary.h"

/* variables declaration */
int IC; /* keeps the lines in the matrix */
int DC; /* keeps the number of commands in the data section  */


/* func to output in the needed special base */
void getOutput(WORD *head, char *fileName){
  WORD *wordPtr = head;
  FILE *outputOB;
  int address = 100;

  outputOB = fopen(strcat(fileName, "Op.ob") ,"w");
  fprintf(outputOB,"\t\t\t%d\t%d\n",IC-100-DC,DC);

  while(wordPtr != NULL && address <= IC){
    binToSpecial(wordPtr->word,address,outputOB);
    wordPtr = wordPtr->next;
    address++;
  }
}

/* func that goes over input and converts to binary but ignores lables */
WORD *goFirstTime(FILE *filePointer, symbolLink *headOfTable){
  char line[81];
  char lineCopy[81];
  char *token;
  char *firstWord;
  symbolLink *lable;
  WORD *headOfFile = (struct WORD*)malloc(sizeof(struct WORD));
  headOfFile->next = NULL;
  IC = 100;
  DC = 0;
  while(fgets(line, 81, filePointer))
  {
    strcpy(lineCopy, line);
    token = strtok(line, " :\t");
    if((lable = findSymbol(headOfTable, token)) != NULL){
      char *tokenCopy = (char *)malloc(81);
      lable->adress = IC;
      token = strtok(NULL, "\n");
      strcpy(tokenCopy, token);
      firstWord = strtok(token, CUT);
      if(isACommand(firstWord))
        toBinaryCommand(tokenCopy, headOfTable, headOfFile);
      else
        toBinaryGuidance(tokenCopy, headOfFile);
    }else if(!strcmp(token, ".extern"))
    {
      token = strtok(NULL, CUT);
      lable = findSymbol(headOfTable, token);
      lable->ext = 1;
      lable->ent = 0;
    }else if(!strcmp(token, ".entry"))
    {
      token = strtok(NULL, CUT);
      lable = findSymbol(headOfTable, token);
      lable->ent = 2;
    }else if(isACommand(cutWhiteChars(token)))
      toBinaryCommand(lineCopy, headOfTable, headOfFile);
    else
      toBinaryGuidance(lineCopy, headOfFile);
  }
  return headOfFile;
}

/* func that adds the needed extra words for all the labels and creates files for ent and ext */
void secondGo(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable, char *fileName){
  char line[81];
  char *token;
  FILE *extFile, *entFile;
  int numOfParsLeft;
  commandsStruct *cmnd;
  symbolLink *symbolFound = NULL;
  symbolLink *current = headOfTable;
  char fileNameCopy[10];
  strcpy(fileNameCopy, fileName);
  extFile = fopen(strcat(fileName, ".ext"), "w");
  entFile = fopen(strcat(fileNameCopy, ".ent"), "w");
  while(fgets(line, 81, filePointer)){
    token = strtok(line, ": \t,");
    if(findSymbol(headOfTable, token) != NULL)
      token = strtok(NULL, " \t,");
    if(isACommand(cutWhiteChars(token))){
      cmnd = findCommand(cutWhiteChars(token));
      numOfParsLeft = cmnd->numOfParam;
      while(numOfParsLeft > 0){
        token = strtok(NULL, "[ \t,");
        if((symbolFound = findSymbol(headOfTable, cutWhiteChars(token))) != NULL)
          changeWord(headOfFile, symbolFound, entFile, extFile);
        numOfParsLeft--;
      }
    }
  }
  while(current != NULL){
    if(current->ent == 2){
      int offset = current->adress % 16;
      fprintf(entFile, "%s,%d,%d\n", current->name, current->adress - offset, offset);
    }
    current = current->next;
  }
}

/* func that frees WORD memory allocation */
void freeWord(WORD *head){
  WORD *current = head;
  WORD *nextOne = head->next;

  while(current != NULL){
    free(current);
    current = nextOne;
    if( current != NULL)
      nextOne = current->next;
  }
}

/* coding a command to the wanted binary */
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile){
  char *command, *token, *restOfString;
  commandsStruct *commandFound = NULL;
  WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
  char lineCopy[81];
  char lineCopy2[81];

  restOfString = (char*)malloc(81);
  makeAllZero(link->word);
  link->next = NULL;

  strcpy(lineCopy, line);
  strcpy(lineCopy2, line);
  command = strtok(line, CUT);
  commandFound = findCommand(command);
  link->word[commandFound->opcode] = 1;
  link->word[18] = 1;
  addWord(headOfFile, link);
  *(restOfString + strlen(restOfString)) = '\0';
  if(commandFound->numOfParam != 0)
    restOfString = strtok(NULL, command);
  deliveryConvertBinary(commandFound, restOfString, headOfTable, headOfFile);
  token = strtok(restOfString, ",\n");

  if(token !=NULL &&(!(*(cutWhiteChars(token)) == 'r' && isARegister(cutWhiteChars(token)) != -1)))
        extraWordsToBinary(cutWhiteChars(token), headOfFile, headOfTable);

  if(commandFound->numOfParam == 2){
    char temp[81];
    int i, j = 0, counter = 0;

    for(i = strlen(lineCopy2)-1; i >=0 && lineCopy2[i] != ',' && lineCopy2[i] != ' '; i--)
      counter++;

    for(i = strlen(lineCopy2) - counter; i <strlen(lineCopy2) ;i++, j++){
      temp[j] = lineCopy2[i];
    }

    if(!(*(cutWhiteChars(temp)) == 'r' && isARegister(cutWhiteChars(temp)) != -1))
        extraWordsToBinary(cutWhiteChars(temp), headOfFile, headOfTable);
  }
}

/* coding a guidance sentence to the wanted binary */
void toBinaryGuidance(char line[], WORD *headOfFile){
  char *guidWord, *param = NULL;
  guidWord = strtok(line, " \t");

  if(!strcmp(guidWord, ".string")){

    int i;
    param = strtok(NULL, "\"");

    for(i = 0; i < strlen(param); i++){
      addWord(headOfFile, charToBinary(*(param + i)));
      DC++;
    }

    addWord(headOfFile, charToBinary('\0'));
    DC++;
    
  }
  else{
    param = strtok(NULL, ",");
    while( param != NULL){

      WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
      int *paramInBinary, i, num = 0, k = 1;
      trimTrailing(param);
      makeAllZero(link->word);

      for(i = strlen(param)-1; i >= 0; i--){
        if(param[i] == '-')
            num = num * (-1);

        else if( isdigit(param[i]) ){
          num += (param[i] - 48) * k;
          k *= 10;
        }
      }
      paramInBinary = decimalToBinary(num);
      link->word[18] = 1;

      for(i = 0; i < 16; i++){
        link->word[i] = *(paramInBinary + i);
      }
      
      addWord(headOfFile, link);
      DC++;
      param = strtok(NULL, ", \n");
    }
  }
}

/* func that distinguishes what address is given and codes it into binary */
void deliveryConvertBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile){ 
  int isDest = 1;
  int i, j;
  char* token, lineCopy[81]; 

  WORD* link =(struct WORD*)malloc(sizeof(struct WORD)); 
  strcpy(lineCopy, myStr);
  token = strtok(myStr, ", \t\n");
  makeAllZero(link->word);
  link->next = NULL;
  link->word[18] = 1;

  if(command->funct != 0){
    int *functInBin;
    functInBin = decimalToBinary(command->funct);
    for(i = 12, j = 0; i < 16; i++, j++)
      link->word[i] = functInBin[j];
  }

  if(command->numOfParam != 0){
    if(command->numOfParam == 2)
      isDest = 0;

    while(token != NULL){
      /* Delivery 0 */ 
      if( token[0] == '#'){
        token = strtok(NULL, ", \t\n");
        continue;
      }

      /* Delivery 1 */ 
      else if(findSymbol(headOfTable, cutWhiteChars(token)) != NULL){
        if(isDest)
          link->word[0] = 1;
        else
          link->word[6] = 1;
      }

      /* Delivery 3 */
      else if(isARegister(cutWhiteChars(token)) != -1){   
        int *regInBinary;
        regInBinary = decimalToBinary(isARegister(token));
        if(isDest){
          for(i = 2, j = 0; i < 6; i++, j++){
            link->word[i] = *(regInBinary + j);
          }
          link->word[0] = 1;
          link->word[1] = 1;
        }
        else{
          for(i = 8, j = 0; i < 12; i++, j++){
            link->word[i] = *(regInBinary + j);
          }

          link->word[6] = 1;
          link->word[7] = 1;
        }
      }
      /*Delivery 2*/
      else{
        int reg;
        int *regInBin;
        reg = extractRegister(token);
        regInBin = decimalToBinary(reg);
        if(isDest){
          for(i = 2, j = 0; i < 6; i++, j++)
            link->word[i] = *(regInBin + j);
          link->word[1] = 1;
        }
        else{
          for(i = 8, j = 0; i < 12; i++, j++)
            link->word[i] = *(regInBin + j);
          link->word[7] = 1;
        }
      }
      if(command->numOfParam == 2 && !isDest){
        char temp[81];
        int i, j = 0, counter = 0;

        for(i = strlen(lineCopy)-1; i >=0 && lineCopy[i] != ',' && lineCopy[i] != ' '; i--)
          counter++;

        for(i = strlen(lineCopy) - counter; i <strlen(lineCopy) ;i++, j++)  
          temp[j] = lineCopy[i];

        token = temp;
        *(token + j) = '\0';
        trimTrailing(token);
      }
      else
        token = NULL;

      isDest = 1;
    }
    addWord(headOfFile, link);
  }
}

/* coding each operand to binary based on the given addressing */
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable)
{
  WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
  makeAllZero(link->word);
  link->next = NULL;

  if(isNum(param)){
    int *numInBinary, i , num = 0, k = 1;
    for(i = strlen(param)-1; i >= 0; i--)
      {
        if(param[i] == '-')
          num = num * (-1);

        else if( isdigit(param[i]) ){
          num += (param[i] - 48) * k;
          k *= 10;
        }
      }
    numInBinary = decimalToBinary(num);

    for(i = 0; i < 16; i++)
      link->word[i] = *(numInBinary + i);

    link->word[18] = 1;
    addWord(headOfFile, link);
  }
  else if(findSymbol(headOfTable,cutWhiteChars(param)) != NULL){
    WORD *link1 = (struct WORD*)malloc(sizeof(struct WORD));
    WORD *link2 = (struct WORD*)malloc(sizeof(struct WORD));
    makeAllZero(link1->word);
    makeAllZero(link2->word);
    addWord(headOfFile, link1);
    addWord(headOfFile, link2);
  }
}

/* as the name implies */
void changeWord(WORD *headOfFile, symbolLink *symbolFound, FILE *entFile, FILE *extFile)
{
  int i, counter = 100;
  WORD *current = headOfFile->next;
  while(current !=NULL){
    if(isAllZero(current)){

      if(symbolFound->ext == 1){
        current->word[16] = 1;
        current->next->word[16] = 1;

        if(symbolFound->times == 1){
          fprintf(extFile, "%s\tBASE\t%d\n", symbolFound->name, counter);
          fprintf(extFile, "%s\tOFFSET\t%d\n\n", symbolFound->name, counter + 1);
          symbolFound->times++;
        }
      } 
      else if(symbolFound->ent == 1 || symbolFound->ent == 2){
        int offset = symbolFound->adress % 16;
        int base = symbolFound->adress - offset;
        int *offsetInBin, *baseInBin;
        current->word[17] = 1;
        current->next->word[17] = 1;
        baseInBin = decimalToBinary(base);
        for(i = 0; i < 16; i++)
          current->word[i] = *(baseInBin + i);
        offsetInBin = decimalToBinary(offset);
        for(i = 0; i < 16; i++)
          current->next->word[i] = *(offsetInBin + i);
      }
      break;
    }
    counter++;
    current = current->next;
  }
}

/* converting a char to the first 16 out of the 20 bits in the struct */
WORD *charToBinary(char ch){
  WORD *link =(struct WORD*)malloc(sizeof(struct WORD));
  int charInAscii = ch, *charInBinary, i;
  makeAllZero(link->word);
  link->word[18] = 1;
  charInBinary = decimalToBinary(charInAscii);
  for(i = 0; i < 16; i++)
      link->word[i] = *(charInBinary + i);
  link->next = NULL;
  return link;
}

/* func that adds a WORD type to linked list */
void addWord(WORD *head, WORD *link){
    while(head->next != NULL)
        head = head->next;
    head->next = link;
    link->next = NULL;
    IC++;
}

/* extracting register's number out */
int extractRegister(char * param){
  char *token;
  token = strtok(param, "[");
  token = strtok(NULL, "]");
  return isARegister(token);
} 

/* func that converts a given int number to the required binary format */
int *decimalToBinary(int num){
  int *array, i;
  array = calloc(16, sizeof(int));

  if( num >= 0 ){
    for(i = 0 ; i < 16 ; i ++){    
      *(array + i) = num%2;    
      num = num/2;    
    }  
    return array;
  }

  num = (~num);

  for(i = 0 ; i < 16 ; i ++){    
    *(array + i) = num%2;    
    num = num/2;    
  }

  for(i = 0 ; i < 16 ; i ++){    
    if( *(array + i) == 1 )
      *(array + i) = 0;
    else if( !*(array + i) )
      *(array + i) = 1;
  }  

  return array;
}

/* initializing the 20 bits in a word struct to zeros */
void makeAllZero(int word[]){
  int i;
  for(i = 0; i < 20; i++)
    word[i] = 0;
}

/* boolean func that checks if all the 20 bits of a WORD have the value 0 */
int isAllZero(WORD *link){
  int i;
  for( i = 0; i < 20; i++){
    if(link->word[i] != 0)
      return 0;
  }
  return 1;
}
