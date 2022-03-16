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

/* check if a given code is valid in assembly */
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
    int go = 1;
    /* the limit of 200 lines is for the makinng of the code */
    /* for every word */
    while( token != NULL && lineNumber <= 200 ){
      if( !wordNumber == 0 )
        token = strtok(NULL, parse_words);
      
      wordNumber++;
      //int go = 1;
      /* Label Declaration */
      if( wordNumber == 1 && token[strlen(token)-1] == ':' ){
        if(!checkForLabelAtBegining(names, token, lineNumber)){
          printf("in line %d: %s is invalid label\n",lineNumber,token);
          result = 0;
        }
        token = strtok(NULL, parse_words);
        wordNumber++;
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
      if(go && token != NULL && has(names,token)){
        NODE* p = getNode(names,token);
        if(p->mac != 0 && countWords(line) == 1){
          go = 0;
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
    
    /* .string */ /* come here */
    if( go && token != NULL && !strcmp(token,".string")){
        int i = 0;
        int j[MAX_LINE_LENGTH];
        int countQM = 0;
        go = 0;
        
        /* token is now .string so we advance it */
        token = strtok(NULL, "\n");
        trimTrailing(token);

        if( token[0] == '\0' ){
          printf("empty string in line: %d\n", lineNumber);
          result = 0;
        }
        token = cutWhiteChars(token);

        /* delete the next line when done */
        /*printf("\n$$$token->@%s@\n",token);*/

        /* converting the chars to ASCII so we wont have the 
            problem that the first " ends the string */
        for( i=0; i < strlen(token); i++ ){
          j[i] = token[i];
          /*printf("index:%d\t char:%c\tvalue:%d\n", i, token[i], j[i]);*/
        }

        /* check the ASCII value of the chars in the array */
        for( i=0; i < strlen(token); i++ ){
          
          if( j[i] == 34 ){
            countQM++; /* count the quotation marks */
            if( i != 0 && i != strlen(token) - 1 ){
              printf("invalid string in line:%d\n", lineNumber);
              result = 0;  
            }
          }
        }
      
        if( result && countQM != 2 ){ 
          printf("invalid string in line:%d\n", lineNumber);
          result = 0;  
        }
        
        if( result && token != NULL ){ 
          token = strtok(NULL, "\t \n");
          
          
          if( token != NULL ){
            printf("invalid string in line:%d\n", lineNumber);
            result = 0;  
          }
        }
      
      }
      /* empty or comment */
      if(go && token != NULL && skip(line)){
        
        go = 0;
      }
      /* command */
      if( go && token != NULL){
        if( isACommand( token ) ){           
          go = 0;
          if(!isRight(line,names)){
            printf("invalid operands in line %d\n",lineNumber);
            result = 0;
          }
        }
      }
      /* if the line is unrecognizable */
      if(go){
        //printf("token2 = %s in line %d\n",token,lineNumber);
        go = 0;
        printf("unrecognizable line in line %d\n",lineNumber);
        result = 0;
        break;
      }
    }
  }
  printf("final result = %d\n",result);
  return result;
}

/* this function is the first pass that parses the
words and filter the special words words */

  /*check if a given command line is valid */
int isRight(char line [], LIST *names){
  char command [MAX_LABEL_LENGTH] = "-1";
  char firstOp [MAX_LABEL_LENGTH] = "-1";
  char secondOp [MAX_LABEL_LENGTH] = "-1";
  char lineCopy[MAX_LINE_LENGTH];
  char *token = "aa";
  int rightCommas = countWords(line) -2;

  strcpy(lineCopy, line);
  token = strtok(lineCopy, parse_words); 
  if(token[strlen(token)-1] == ':'){
    token = strtok(NULL, parse_words);
    rightCommas--;
  }
  if(rightCommas < 0){
    rightCommas = 0;
  }
  if(!isValidCommas(rightCommas,line) || rightCommas > 1){
    return 0;
  }
  int i = 0;
  while(token != NULL){
    
    if(i==0)
      strcpy(command,token);
    if(i==1)
      strcpy(firstOp,token);
    if(i==2)
      strcpy(secondOp,token);
    if(i==3)
      return 0;
    
    i++;
    token = strtok(NULL, " ,\t\n");
  }

  return validOperands(command,firstOp,secondOp,names);
}

/* returns the valid macro and label as a list */
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


/* given a command find how many operands */
/* delete? */
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

/* can delete it */
int isCommandLegit( char lineCopy[], int operandsNum, int wordNumber, LIST *names, int lineNumber ){
  char *token;
  char *command_name;
  int operand_index = 0;
  int result = 1;
  char SecondOp [30] = "-1";
  char firstOp [30] = "-1";
  //printf("1\n");
  trimTrailing(lineCopy);
  //printf("lineCopy = %s\n",lineCopy);
 // printf("wordNumber = %d\n",wordNumber);
  //printf("operandsNum = %d\n",operandsNum);
  if(wordNumber == 1 || wordNumber == 2){
    int i, iter;
    int wordsInLine = countWords(lineCopy);

    /* checking valid though the number of lines */
    if(wordNumber == 2)
      //token = strtok( lineCopy, "\n ," );
      wordsInLine--; /* for comparing it later to count commas */
    
    if( wordsInLine - 1 != operandsNum){
      printf("too many or too few words in line: %d\n", lineNumber);
      return 0;
    }

    /* token initialization and advancement */
    token = strtok( lineCopy, "\n ," );
    //printf("1token = %s\n",token);
    if( isACommand(token) ){
      trimTrailing(token);
      strcpy(command_name, token);
      iter = 1;
    }
      
    else
      iter = 2;

    for( i = 0; i < iter; i++ ){
      token = strtok( NULL, "\n ," );

      if( token == NULL )
        result = 0;
    }
    while( result && token != NULL ){
      printf("token = %s\n",token);
      int delivery_num;

      trimTrailing(token);
      operand_index++;

      if(operand_index == 1){
        strcpy(firstOp,token);
      }
      if(operand_index == 2){
        strcpy(SecondOp,token);
      }
      //printf("operand index %d is->%s@\n", operand_index,token);
      token = strtok( NULL, "\n ," ); 
    }
     printf("firstOp = %s\n",firstOp);
     printf("SecondOp = %s\n",SecondOp);
     printf("command_name = %s\n",command_name);

  }
  return 1;
}

/* func to decide which delivery it is
   the given string is after move to none white!*/
int whichDelivery(char myStr[], LIST *names){
  char tempLine[MAX_LABEL_LENGTH];
  char tempLabel[MAX_LABEL_LENGTH];
  char tempReg[MAX_LABEL_LENGTH];
  char* token;
  
  if( myStr == NULL ){
    return 0;
  }
  
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
 
  token = strtok(NULL, parse_delivery);
  if(token == NULL)
    return -1;
  strcpy( tempReg, token );
  if( strlen(tempReg) == 4 ){
      if( tempReg[0] == 'r' && tempReg[1] == '1' && tempReg[3] == ']' ){
        if( tempReg[2] >= '0' && tempReg[2] <= '5' ){
          if( has( names, tempLabel) && getNode(names,tempLabel)->mac == 0 ){
              return 2;
            }
          }
      } 
  }
  else
    return -1;
}

/* delete it? */
int isAGuidance(char line []){
  LIST *l = newList();
  insert(l,".data");
  insert(l,".string");
//insert(l,".entry");
//  insert(l,".extern");
  return has(l,line);
}

/* delete it? */
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

/* delete it? */
char* getRidOfFirstChar(char myStr[]){
  return myStr + 1;
}

/* check if a given string is a command */
int isACommand(char token[] ){
  if( !strcmp(token,"mov") || !strcmp(token,"cmp") || !strcmp(token,"add") || !strcmp(token,"sub") || !strcmp(token,"lea") || !strcmp(token,"clr") || !strcmp(token,"not") || !strcmp(token,"inc") || !strcmp(token,"dec") || !strcmp(token,"jmp") || !strcmp(token,"bne") || !strcmp(token,"jsr") || !strcmp(token,"red") || !strcmp(token,"prn") || !strcmp(token,"rts") || !strcmp(token,"stop") )
    return 1;

  return 0;
}

/* check if a given string is a register */
int isARegister(char line[]){
  move_to_none_white(line, 0);
  /* trimTrailing(line); */ /* created segmention fault */
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

/* check if a given string is a valid name */
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

/* need to solve the option of two identical macros */
/* check if a given string is a valid macro */
int checkForMacroAtSecond( LIST *names, char token[], int lineNumber ){

  if( has(names, token) ){
    if( getNode(names, token)->mac == 1 ){
      getNode(names, token)->mac = 2;
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

int validOperands(char com [], char firstOp [], char secondOp [], LIST *names){
  //printf("com = %s \tfirstOp = %s\tsecondOp = %s\n",com,firstOp,secondOp);
    
  int op1 = -1;
  int op2 = -1;
  int res1 = 0;
  int res2 = 0;
  //printf("i've been here\n");
  trimTrailing(firstOp);
  if( firstOp[0] != '\0' ){
    op1 = whichDelivery(firstOp,names);
  } 
  
  trimTrailing(secondOp);
  if( secondOp[0] != '\0' ){
    
    op2 = whichDelivery(secondOp,names);
  }
  /* op1 = 0,1,2,3 */ if(!strcmp(com,"mov")||!strcmp(com,"cmp")||!strcmp(com,"add")||!strcmp(com,"sub")){
    if(op1 != -1){
      res1 = 1;
      
    }
  }
  if(!strcmp(com,"prn") && op1 != -1 && !strcmp(secondOp,"-1"))
    return 1;
  
  /* op1 = 1,2,3  (they have only 1 operand) */
  if(!strcmp(com,"clr")||!strcmp(com,"not")||!strcmp(com,"inc")|| !strcmp(com,"dec")){
    if(op1 != -1 && op1 != 0 && !strcmp(secondOp,"-1"))
      return 1;
  }
  if(!strcmp(com,"red") && op1 != -1 && op1 != 0 && !strcmp(secondOp,"-1"))
    return 1;
  
  /* op1 = 1,2 */
  if(!strcmp(com,"lea") && (op1 == 1 || op1 == 2))
    res1 = 1;
  
  if(!strcmp(com,"jmp") || !strcmp(com,"bne") || !strcmp(com,"jsr")){
    if((op1 == 1 || op1 == 2) && !strcmp(secondOp,"-1")){
      return 1;
    }
  }
  /* no operands */
  if(!strcmp(com,"rts") || !strcmp(com,"stop")){
    if(!strcmp(firstOp,"-1") && !strcmp(secondOp,"-1")){
      return 1;
    }
  }
  /* op2 = 0,1,2,3*/
  if(!strcmp(com,"cmp") && op2 != -1)
    res2 = 1;
  
  /* op2 = 1,2,3 */
  if(!strcmp(com,"mov") || !strcmp(com,"add") || !strcmp(com,"sub") ||!strcmp(com,"lea")){
    /* problem if there is invalid label as operand */
    
    //printf("com = %s\t op1 = %d\top2 = %d\n",com,op1,op2);
    if(op2 != 0 && op2 != -1){
      res2 = 1;
    }
  }
  
  if(res1 && res2)
    return 1;
  
  return 0;
}
