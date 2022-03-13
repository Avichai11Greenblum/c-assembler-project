#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "list.h"
#include "validation.h"

#define parse_words "  \t\n"
#define parse_delivery " ,\t\n["
#define parse_commands " ,\t\n:"


int validation(FILE *filePtr, LIST *names){
  int result = 1, lineNumber = 0;
  int commaLegit = 0, neededOperands;
  char line[MAX_LINE_LENGTH];
  char *token = "aa";
  int numOfValidMacros = 0;
  
  fseek(filePtr, 0, SEEK_SET);
  /* for every line */
  while (fgets(line, MAX_LINE_LENGTH, filePtr)){
    char lineCopy[MAX_LINE_LENGTH];
    int wordNumber = 0;
    lineNumber++;
    strcpy(lineCopy, line);
    token = strtok(lineCopy, parse_words); 
    /* the limit of 200 lines is for the makinng of the code */
    /* for every word */
    while( token != NULL && lineNumber <= 200 ){
      int go = 1;
      if( !wordNumber == 0 )
        token = strtok(NULL, parse_words);
      
      wordNumber++;
      
      /* Label Declaration */
      if( wordNumber == 1 && token[strlen(token)-1] == ':' ){
        if(!checkForLabelAtBegining(names, token, lineNumber)){
          printf("in line %d: %s is invalid label\n",lineNumber,token);
          result = 0;
        }
      }
      
      /* macro */
      if( go && token != NULL){
        trimTrailing(token);
        
        if( wordNumber == 1 && !strcmp(token,"macro")){
          go = 0;
          token = strtok(NULL, parse_words);
          wordNumber++;
          if(checkForMacroAtSecond(names, token, lineNumber) == 0 || isCurNumOfWords( line, 2) == 0){
            result = 0;
            printf("in line %d: invalid macro\n",lineNumber);
          }
          else{
            numOfValidMacros++;
          }
        }
      }

      /* endm */
      if( go && token != NULL && !strcmp(token,"endm")){
        go = 0;
        if(isCurNumOfWords(line,1)  && numOfValidMacros != 0){
          numOfValidMacros--;
        }
        else{
          printf("in line %d: invalid endm\n",lineNumber);
          result = 0;
        }
      }
      
      /* extern */
      if( go && token != NULL && !strcmp(token,".extern")){
        go = 0;
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(!isCurNumOfWords( line, 2) || !checkForExternAtSecond(names, token, lineNumber)){
          printf("in line %d: invalid extern\n",lineNumber);
          result = 0;
        }
      }

      /* entry */
      if( go && token != NULL && !strcmp(token,".entry")){
          go = 0;
          token = strtok(NULL, parse_words);
          wordNumber++;
        if(!isCurNumOfWords( line, 2) || !checkForEntryAtSecond(names, token, lineNumber) ){
          printf("in line %d: invalid entry\n",lineNumber);  
          result = 0;
          }
      }
      
      /* .data */
      if( go && token != NULL){
        if( !strcmp(token,".data") ){
          go = 0;
          if(wordNumber == 1 || wordNumber == 2){
            int wordsInLine = countWords(line);
            
            if(wordNumber == 2){
              wordsInLine--; /* for comparing it later to count commas */
            }
            
            while( token != NULL ){
  
              token = strtok( NULL, "\n ," ); 
              if( token == NULL )
                break;
              trimTrailing(token);
              
              
              if( !isAIntNum(token) ){
                printf("line %d: is not a number!\n", lineNumber);
                result = 0;
              } 
            }
            
            if(!isValidCommas(wordsInLine-2,line)){
              printf("line %d: invalid commas!\n", lineNumber);
              result = 0;
            }
          }
        }
      }
    
      /* .string */
      if( go && token != NULL && !strcmp(token,".string")){
          go = 0;
          trimTrailing(line);
          if( countQuotationMarks(line) != 2 || line[strlen(line)-1] != '"' ){
            printf("invalid string in line %d\n", lineNumber);
            result = 0;
          }
        }
      /* empty or comment */
      if(go && token != NULL && skip(line)){
        go = 0;
      }
      /* command */
      if( go && token != NULL){
        if( isACommand( token ) ){
          int operandsNum = howManyOperands(token);
          char lineCopyCommand[31];
          strcpy(lineCopyCommand, line);
          
          go = 0;
          
          if( !isCommandLegit(lineCopy, operandsNum, wordNumber, names, lineCopyCommand) )
            result = 0;
          
        }
      }
    }
  }
  return result;
}

/* this function is the first pass that parses the
words and filter the special words words */

/* need to fix a bug - if there is empty line in the input we have an error */
LIST *validNames(FILE *fileName, char *nameOfFile){
  char line[MAX_LINE_LENGTH];
  char lineCopy[MAX_LINE_LENGTH];
  char *token;
  LIST *names = newList();
  
  while (fgets(line, MAX_LINE_LENGTH, fileName)){
    strcpy(lineCopy, line);
    
    if(!skip(line)){
      token = strtok(lineCopy, parse);

      /* macro */
      if( !strcmp(token, "macro") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse); 
        insertMa(token,names);
      }

      /* extern */
      if( !strcmp(token, ".extern") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse);
        insertEx(token,names);
      }

      /* entry */
      if( !strcmp(token, ".entry") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse);
        insertEn(token,names);
      }

      /* label declaration */
      if(token[strlen(token)-1] == ':'){
        token[strlen(token)-1] = '\0';
        insertLD(token,names);
      }
    }
  }
  return names;
}

int checkForLabelAtBegining( LIST *names, char token[], int lineNumber ){

  if(token[strlen(token)-1] == ':'){
    token[strlen(token)-1] = '\0';
    
    if( has( names, token) ){
      if( getNode(names, token)->labDec == 1 ){
        getNode(names, token)->labDec = 0;
        return 1;
      }
    }
  }
  return 0;
}

int checkForMacroAtSecond( LIST *names, char token[], int lineNumber ){

  if( has(names, token) ){
    if( getNode(names, token)->mac == 1 ){
      getNode(names, token)->mac = 0;
      return 1;
    }
  }
  return 0;
}

int checkForExternAtSecond( LIST *names, char token[], int lineNumber ){
    if( has(names, token) ){
    if( getNode(names, token)->ext != 0 ){
      return 1;
    }
  }
  return 0;
}

int checkForEntryAtSecond( LIST *names, char token[], int lineNumber ){
    if( has(names, token) ){
    if( getNode(names, token)->ent != 0 ){
      return 1;
    }
  }
  return 0;
}

/* given a command find how many */
int howManyOperands( char commandName[] ){
  if( !strcmp(commandName, "mov") || !strcmp(commandName, "cmp") || !strcmp(commandName, "add") || !strcmp(commandName, "sub") || !strcmp(commandName, "lea") ){
    return 2;
  }

  if( !strcmp(commandName, "rts") || !strcmp(commandName, "stop") )
    return 0;

  if( !strcmp(commandName, "clr") || !strcmp(commandName, "not") || !strcmp(commandName, "inc") || !strcmp(commandName, "dec") || !strcmp(commandName, "jmp") || !strcmp(commandName, "bne") || !strcmp(commandName, "jsr") || !strcmp(commandName, "red") || !strcmp(commandName, "prn") ){
    return 1;
  }

  return -1;
}

int isCommandLegit( char lineCopy[], int operandsNum, int wordNumber, LIST *names, int lineNumber ){
  char *token;
  int operand_index = 0;
  int result = 1;
  if(wordNumber == 1 || wordNumber == 2){
    int wordsInLine = countWords(lineCopy);
    
    if( wordsInLine - 1 != operandsNum && wordsInLine - 2 != operandsNum )
      result = 0;

    if(wordNumber == 2){
      wordsInLine--; /* for comparing it later to count commas */
    }
    
    printf("line copy:%s\n", lineCopy);
    printf("1\n");
    printf("token:%s@\n", token);
    printf("2\n");
    
    while( result && token != NULL ){
      int delivery_num;

      token = strtok( NULL, "\n ," ); 
      if( token == NULL )
        break;
      trimTrailing(token);
      operand_index++;
      
      /* token is now a operand */
      delivery_num = whichDelivery(token, names);
      if( delivery_num == -1 ){
        result = 0;
        printf("invalid addressing line:%d", lineNumber);
        break;
      }

      printf("operand number %d, is:%s", operand_index, token);
    }
    
    if(!isValidCommas(wordsInLine-2,lineCopy)){
      result = 0;
      printf("line %d: invalid commas!\n", lineNumber);
    }
  }
  return 1;
}

/* func to decide which delivery it is
   the given string is after move to none white!*/
int whichDelivery(char myStr[], LIST *names){
  char tempLine[30];
  char tempLabel[30];
  char tempRegister[30];
  char* token;
  
  /* Delivery 0 */
  if( myStr[0] == '#' && isAIntNum(myStr + 1)){
    return 0;
  }
  /* Delivery 1 */
  if(has(names,myStr) && getNode(names,myStr)->mac == 0){
    return 1;
  }

  /* Delivery 3 */
  if( isARegister(myStr) != -1 ){
    return 3;
  }
  
  /* Delivery 2 */
  strcpy(tempLine, myStr);
  token = strtok(tempLine, parse_delivery);
  strcpy( tempLabel, token );
  printf("\nlabel->%s", tempLabel);
 
  token = strtok(NULL, parse_delivery);
  if(token == NULL)
    return -1;
  strcpy( tempRegister, token );
  printf("\nregister->%s", tempRegister);

  if( strlen(tempRegister) == 4 ){
      if( tempRegister[0] == 'r' && tempRegister[1] == '1' && tempRegister[3] == ']' ){
  
          if( tempRegister[2] == '0' || tempRegister[2] == '1' || tempRegister[2] == '2' || tempRegister[2] == '3' ||         tempRegister[2] == '4' ||         tempRegister[2] == '5' ){

            if( has( names, tempLabel) ){
              return 2;
            }
          }
      } 
  }

  else
    return -1;
}

int isAGuidance(char line []){
  LIST *l = newList();
  insert(l,".data");
  insert(l,".string");
//insert(l,".entry");
//  insert(l,".extern");
  return has(l,line);
}
  
int isACommentOrEmpty(char line []){
  int i = 0;
  while(line[i] != '\0'){
    if(isWhiteSpace(line[i]))
      i++;
    else if(line[i] == ';')
      return 1;
    else
      return 0;
  }
  return 1;
}

/* as easy as it sounds */
char* getRidOfFirstChar(char myStr[]){
  return myStr + 1;
}

int isACommand(char token[] ){
  if( !strcmp(token,"mov") || !strcmp(token,"cmp") || !strcmp(token,"add") || !strcmp(token,"sub") || !strcmp(token,"lea") || !strcmp(token,"clr") || !strcmp(token,"not") || !strcmp(token,"inc") || !strcmp(token,"dec") || !strcmp(token,"jmp") || !strcmp(token,"bne") || !strcmp(token,"jsr") || !strcmp(token,"red") || !strcmp(token,"prn") || !strcmp(token,"rts") || !strcmp(token,"stop") )
    return 1;

  return 0;
}

int isARegister(char line[]){
  move_to_none_white(line, 0);
  trimTrailing(line);
  if(!strcmp(line,"r0"))
    return 0;  
  if(!strcmp(line,"r1"))
    return 1;
  if(!strcmp(line,"r2"))
    return 2;  
  if(!strcmp(line,"r3"))
    return 3;
  if(!strcmp(line,"r4"))
    return 4;  
  if(!strcmp(line,"r5"))
    return 5;
  if(!strcmp(line,"r6"))
    return 6;  
  if(!strcmp(line,"r7"))
    return 7;
  if(!strcmp(line,"r8"))
    return 8;  
  if(!strcmp(line,"r9"))
    return 9;
  if(!strcmp(line,"r10"))
    return 10;  
  if(!strcmp(line,"r11"))
    return 11;
  if(!strcmp(line,"r12"))
    return 12;  
  if(!strcmp(line,"r13"))
    return 13;
  if(!strcmp(line,"r14"))
    return 14;  
  if(!strcmp(line,"r15"))
    return 15;
  
  return -1;
}

/* is name valid or not */
int isNameOk(char line []){
  int i = 0;
  if(!isalpha(line[i]) || isARegister(line) != -1 || isACommand(line) || strlen(line) > MAX_LABEL_LENGTH){
    return 0;
  }
  while(line[i] != '\0'){
    if(!isalpha(line[i]) && !isANaturalNum(line[i])){
      return 0;
    }
    i++;
  }
  return 1;
}

/* insert macro */
void insertMa(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->mac++;
    }
  }
}

/* insert extern */
void insertEx(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->ext++;
    }
    else{
      NODE* p = getNode(names,token);
      if(p->mac == 0 && p->ent == 0 && p->labDec == 0){
        p->ext++; 
      }
    }
  }
}

/* insert enrty */
void insertEn(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->ent++;
    }
    else{
      NODE* p =getNode(names,token);
      if(p->mac == 0 && p->ext == 0){
        p->ent++; 
      }
    }
  }
}

/* insert label declaration */
void insertLD(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->labDec++;
    }
    else{
      NODE* p =getNode(names,token);
      if(p->mac == 0 && p->ext == 0 && p->labDec == 0){
        p->labDec++; 
      }
    }
  }
}

