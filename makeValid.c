#include "makeValid.h"


/* check if a given code is valid in assembly */ 
int validation(FILE *filePtr, LIST *names){
  int result = 1, lineNumber = 0;
  char line[MAX_LINE_LENGTH];
  char *token = "text";
  int numOfValidMacros = 0;
  
  fseek(filePtr, 0, SEEK_SET);

  /* for every line */
  while (fgets(line, MAX_LINE_LENGTH, filePtr)){
    char lineCopy[MAX_LINE_LENGTH];
    int wordNumber = 0;
    int checkOther = 1;
    lineNumber++;
    strcpy(lineCopy, line);
    token = strtok(lineCopy, parse_words); 
    
    /* for every word */
    while( token != NULL){
      if( wordNumber != 0 )
        token = strtok(NULL, parse_words);
      
      wordNumber++;
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
      if( checkOther && token != NULL){
        trimTrailing(token);
        if( wordNumber == 1 && !strcmp(token,"macro")){
          checkOther = 0;
          token = strtok(NULL, parse_words);
          wordNumber++;
          if(checkForMacroAtSecond(names, token, lineNumber) == 0 || isCurNumOfWords( line, 2) == 0){
            result = 0;
            printf("in line %d: invalid macro\n",lineNumber);
          }else
            numOfValidMacros++;
        }
      }
      if(checkOther && token != NULL && checkList(names,token)){
        NODE* p = getNode(names,token);
        if(p->mac != 0 && countWords(line) == 1)
          checkOther = 0;
      }
      /* endm */
      if( checkOther && token != NULL && !strcmp(token,"endm")){
        checkOther = 0;

        if(isCurNumOfWords(line,1)  && numOfValidMacros != 0)
          numOfValidMacros--;

        else{
          printf("in line %d: invalid endm\n",lineNumber);
          result = 0;
        }
      }
      
      /* extern */
      if( checkOther && token != NULL && !strcmp(token,".extern")){
        checkOther = 0;
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(!isCurNumOfWords( line, 2) || !checkForExternAtSecond(names, token, lineNumber)){
          printf("in line %d: invalid extern\n",lineNumber);
          result = 0;
        }
      }

      /* entry */
      if( checkOther && token != NULL && !strcmp(token,".entry")){
        checkOther = 0;
        token = strtok(NULL, parse_words);
        wordNumber++;

        if(!isCurNumOfWords( line, 2) || !checkForEntryAtSecond(names, token, lineNumber)){
          printf("in line %d: invalid entry\n",lineNumber);  
          result = 0;
        }
      }
      
      /* .data */
      if( checkOther && token != NULL){
        int labelExists = 0;

        if( !strcmp(token,".data") ){
          checkOther = 0;

          if(wordNumber == 1 || wordNumber == 2){
            int wordsInLine = countWords(line);
            
            if(wordNumber == 2)
              labelExists = 1;
              /*wordsInLine--;  for comparing it later to count commas */
            
            while( token != NULL ){
              token = strtok( NULL, "\n ," ); 
              if( token == NULL )
                break;
              trimTrailing(token);
              if(!isAIntNum(token)){
                printf("line %d: is not a number!\n", lineNumber);
                result = 0;
              } 
            }

            if(labelExists){
              if(!isValidCommas(wordsInLine-3,line) ){
                printf("line %d: invalid commas!\n", lineNumber);
                result = 0;
              }
            }
            else{
              if(!isValidCommas(wordsInLine-2,line))
              {
                printf("line %d: invalid commas!\n", lineNumber);
                result = 0;
              }
            }
          }
        }
      }
    
    /* .string */ /* come here */
    if( checkOther && token != NULL && !strcmp(token,".string")){
        int i = 0;
        int j[MAX_LINE_LENGTH];
        int countQM = 0;
        checkOther = 0;
        
        /* token is now .string so we advance it */
        token = strtok(NULL, "\n");
        trimTrailing(token);

        if( token[0] == '\0' ){
          printf("empty string in line: %d\n", lineNumber);
          result = 0;
        }
        token = cutWhiteChars(token);

        /* converting the chars to ASCII so we wont have the 
            problem that the first " ends the string */
        for( i=0; i < strlen(token); i++ )
          j[i] = token[i];

        /* check every ASCII value if its equal to quotation marks */
        for( i=0; i < strlen(token); i++ ){  
          if( j[i] == 34 ){
            /* count the quotation marks */
            countQM++; 

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
      if(checkOther && token != NULL && skip(line))
        checkOther = 0;
      
      /* command */
      if( checkOther && token != NULL){
        if(isACommand(token)){           
          checkOther = 0;
          if(!isRight(line,names)){
            printf("invalid operands in line %d\n",lineNumber);
            result = 0;
          }
        }
      }
      /* if the line is unrecognizable */
      if(checkOther){
        checkOther = 0;
        printf("unrecognizable line in line %d\n",lineNumber);
        result = 0;
        break;
      }
    }
  }
  return result;
}

/* check if a given command line is valid */
int isRight(char line [], LIST *names){
  char command [MAX_LABEL_LENGTH] = "-1";
  char firstOp [MAX_LABEL_LENGTH] = "-1";
  char secondOp [MAX_LABEL_LENGTH] = "-1";
  char lineCopy[MAX_LINE_LENGTH];
  char *token = "aa";
  int rightCommas = countWords(line) -2;
  int i = 0;

  strcpy(lineCopy, line);
  token = strtok(lineCopy, parse_words); 

  if(token[strlen(token)-1] == ':'){
    token = strtok(NULL, parse_words);
    rightCommas--;
  }

  if(rightCommas < 0)
    rightCommas = 0;

  if(!isValidCommas(rightCommas,line) || rightCommas > 1)
    return 0;

  while(token != NULL){
    if(i==0)
      strcpy(command,token);
    else if(i==1)
      strcpy(firstOp,token);
    else if(i==2)
      strcpy(secondOp,token);
    else if(i==3)
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
  
names->head = NULL;
  while (fgets(line, MAX_LINE_LENGTH, fileName)){

    strcpy(lineCopy, line);
    
    if(!skip(line)){
      token = strtok(lineCopy, parse);

      /* macro */
      if( !strcmp(token, "macro") && countWords(line) == 2){
        token = strtok(NULL, parse);
        insertMa(token,names);
      }

      /* extern */
      else if( !strcmp(token, ".extern") && countWords(line) == 2){
        token = strtok(NULL, parse);
        insertEx(token,names);
      }

      /* entry */
      else if( !strcmp(token, ".entry") && countWords(line) == 2){
        token = strtok(NULL, parse);
        insertEn(token,names);
      }

      /* label declaration */
      else if(token[strlen(token)-1] == ':'){
        token[strlen(token)-1] = '\0';
        insertLD(token,names);
      }
    }
  }
  return names;
}

/* func to decide which delivery it is
   the given string is after move to none white!*/
int whichDelivery(char myStr[], LIST *names){
  char tempLine[MAX_LABEL_LENGTH];
  char tempLabel[MAX_LABEL_LENGTH];
  char tempReg[MAX_LABEL_LENGTH];
  char* token;
  
  if( myStr == NULL )
    return 0;
  
  /* Delivery 0 */
  if( myStr[0] == '#' && isAIntNum(myStr + 1))
    return 0;

  /* Delivery 1 */
  if(checkList(names,myStr) && getNode(names,myStr)->mac == 0)
    return 1;

  /* Delivery 3 */
  if( isARegister(myStr) != -1 )
    return 3;

  /* Delivery 2 */
  strcpy(tempLine, myStr);
  token = strtok(tempLine, parse_delivery);
  strcpy( tempLabel, token );
 
  token = strtok(NULL, parse_delivery);
  if(token == NULL)
    return -1;
  strcpy( tempReg, token );
  if( strlen(tempReg) == 4 ){
      if( tempReg[0] == 'r' && tempReg[1] == '1' && tempReg[3] == ']' )
        if( tempReg[2] >= '0' && tempReg[2] <= '5' )
          if( checkList( names, tempLabel) && getNode(names,tempLabel)->mac == 0 )
              return 2;
  }
  return -1;
}

/* check if a given string is a valid macro */
int checkForMacroAtSecond( LIST *names, char token[], int lineNumber ){
  if(checkList(names, token)){
    if( getNode(names, token)->mac == 1 ){
      getNode(names, token)->mac = 2;
      return 1;
    }
    return 0;
  }
  return 0;
}

int checkForExternAtSecond( LIST *names, char token[], int lineNumber ){
  if(checkList(names, token))
    if(getNode(names, token)->ext != 0)
      return 1;
  return 0;
}

int checkForEntryAtSecond( LIST *names, char token[], int lineNumber ){
  if(checkList(names, token))
    if(getNode(names, token)->ent != 0)
      return 1;
  return 0;
}

int checkForLabelAtBegining( LIST *names, char token[], int lineNumber ){
  if(token[strlen(token)-1] == ':'){
    token[strlen(token)-1] = '\0';

    if(checkList( names, token)){
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
    if(!checkList(names,token)){
      insert(names,token);
      names->head->mac++;
    }
  }
}

/* insert extern */
void insertEx(char token [], LIST* names){
  if(isNameOk(token)){
    if(!checkList(names,token)){
      insert(names,token);
      names->head->ext++;
    }
    else{
      NODE* p = getNode(names,token);
      if(p->mac == 0 && p->ent == 0 && p->labDec == 0)
        p->ext++; 
    }
  }
}

/* insert enrty */
void insertEn(char token [], LIST* names){

  if(isNameOk(token)){
    if(!checkList(names,token)){
      insert(names,token);
      names->head->ent++;

    }
    else{
      NODE* p =getNode(names,token);
      if(p->mac == 0 && p->ext == 0)
        p->ent++; 
    }
  }
}

/* insert label declaration */
void insertLD(char token [], LIST* names){
  if(isNameOk(token)){
    if(!checkList(names,token)){
      insert(names,token);
      names->head->labDec++;
    }
    else{
      NODE* p = getNode(names,token);
      if(p->mac == 0 && p->ext == 0 && p->labDec == 0)
        p->labDec++; 
    }
  }
}

int validOperands(char com [], char firstOp [], char secondOp [], LIST *names){    
  int op1 = -1;
  int op2 = -1;
  int res1 = 0;
  int res2 = 0;

  trimTrailing(firstOp);
  if( firstOp[0] != '\0' )
    op1 = whichDelivery(firstOp,names);
  
  trimTrailing(secondOp);
  if( secondOp[0] != '\0' )
    op2 = whichDelivery(secondOp,names);
  
  /* op1 = 0,1,2,3 */ 
  if(!strcmp(com,"mov")||!strcmp(com,"cmp")||!strcmp(com,"add")||!strcmp(com,"sub"))
    if(op1 != -1)
      res1 = 1;
  
  if(!strcmp(com,"prn") && op1 != -1 && !strcmp(secondOp,"-1"))
    return 1;
  
  /* op1 = 1,2,3  (they have only 1 operand) */
  if(!strcmp(com,"clr")||!strcmp(com,"not")||!strcmp(com,"inc")|| !strcmp(com,"dec"))
    if(op1 != -1 && op1 != 0 && !strcmp(secondOp,"-1"))
      return 1;
  
  if(!strcmp(com,"red") && op1 != -1 && op1 != 0 && !strcmp(secondOp,"-1"))
    return 1;
  
  /* op1 = 1,2 */
  if(!strcmp(com,"lea") && (op1 == 1 || op1 == 2))
    res1 = 1;
  
  if(!strcmp(com,"jmp") || !strcmp(com,"bne") || !strcmp(com,"jsr"))
    if((op1 == 1 || op1 == 2) && !strcmp(secondOp,"-1"))
      return 1;
    
  /* no operands */
  if(!strcmp(com,"rts") || !strcmp(com,"stop"))
    if(!strcmp(firstOp,"-1") && !strcmp(secondOp,"-1"))
      return 1;
  
  /* op2 = 0,1,2,3*/
  if(!strcmp(com,"cmp") && op2 != -1)
    res2 = 1;
  
  /* op2 = 1,2,3 */
  if(!strcmp(com,"mov") || !strcmp(com,"add") || !strcmp(com,"sub") ||!strcmp(com,"lea"))
    if(op2 != 0 && op2 != -1)
      res2 = 1;
  
  if(res1 && res2)
    return 1;
  
  return 0;
}
