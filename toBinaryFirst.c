#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "firstPass.h"

int isACommand(char line[] ){
  if(!strcmp(line,"mov"))
    return 1;
  if(!strcmp(line,"cmp"))
    return 1;
  if(!strcmp(line,"add"))
    return 1;
  if(!strcmp(line,"sub"))
    return 1;
  if(!strcmp(line,"lea"))
    return 1;  
  if(!strcmp(line,"clr"))
    return 1;
  if(!strcmp(line,"not"))
    return 1;
  if(!strcmp(line,"inc"))
    return 1;
  if(!strcmp(line,"dec"))
    return 1;
  if(!strcmp(line,"jmp"))
    return 1;
  if(!strcmp(line,"bne"))
    return 1;  
  if(!strcmp(line,"jsr"))
    return 1;
  if(!strcmp(line,"red"))
    return 1;
  if(!strcmp(line,"prn"))
    return 1;
  if(!strcmp(line,"rts"))
    return 1;
  if(!strcmp(line,"stop"))
    return 1;
  return 0;
}


commandsStruct *findCommand(char * command)
{
    int i = 0;
    for(; i < 17; i++)
    {
        if(!strcmp(command, ourCommands[i].commandName))
        	return &ourCommands[i];
    }
    return NULL;

}

WORD *deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable)
{ 
    int isDest = 1;
    int i, j;
    char* token; 
    WORD* link = NULL; 
    
    token = strtok(myStr, ", \t\n");
    link->word[18] = 1;
    if(command->funct != 0)
    {
        int *functInBin;
        functInBin = decToBinary(command->funct);
        for(i = 12, j = 0; i < 16; i++, j++)
            link->word[i] = functInBin[j];
    }

    if(command->numOfParam == 0)
        return link;

    while(token != NULL)
    {
        /* Delivery 0 */ 
        if( myStr[0] == '#')
        {
            token = strtok(NULL, ", \t\n");
            continue;
        }

        /* Delivery 1 */ 
        else if(findSymbol(headOfTable, myStr) != NULL)
            if(isDest)
                link->word[0] = 1;
            else
                link->word[6] = 1;

        /* Delivery 3 */
        else if(isARegister(cutWhiteChars(token)) != -1)
        {   
            int *regInBinary;
            regInBinary = decToBinary(isARegister(token));
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = *(regInBinary + j);
                link->word[0] = 1;
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = *(regInBinary + j);
                link->word[6] = 1;
                link->word[7] = 1;
            }
        }

        /*Delivery 2*/
        else
        {
            int reg;
            int *regInBin;
            reg = extractRegister(token);
            regInBin = decToBinary(reg);
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = *(regInBin + j);
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = *(regInBin + j);
                link->word[7] = 1;
            }
        }
        isDest = 0;
        token = strtok(NULL, ",\n");
    }
    IC++;
    return link;
}

char *cutWhiteChars(char *str)
{
    char *newStr;
    int i, j = 0;
    newStr = malloc(strlen(str) * sizeof(char));
    for(i = 0; i < strlen(str); i++)
    {
        if(!isspace(*(str + i)))
        {
            *(newStr + j) = *(str + j);
            j++;
        }
    }
    *(newStr + j) = '\0';
    return newStr;
}

LINE *toBinaryCommand(char line[], symbolLink *headOfTable)
{
    char *command;
    char *token;
    WORD *headForLine = (struct WORD*)malloc(sizeof(struct WORD));
    LINE *node = NULL;
    commandsStruct *commandFound;
    char *restOfString;
    node->wordHead = NULL;

    command = strtok(line, CUT);
    commandFound = findCommand(command);
    headForLine->word[commandFound->opcode] = 1;
    headForLine->word[18] = 1;
    IC++;
    node->wordHead = headForLine;
    restOfString = strtok(NULL, "\n");

    addWord( headForLine, deliveryForBinary(commandFound, restOfString, headOfTable));
    
    token = strtok(restOfString, ",\n");

    while(token != NULL)
    {
        if(isARegister(cutWhiteChars(token)) == -1)
            break;
        addWord(headForLine, extraWordsToBinary(token));
        token = strtok(NULL, ",\n");
    }
    return node;
}


LINE *firstPass(FILE *filePointer, symbolLink *headOfTable)
{
    char line[81];
    char lineCopy[81];
    char *token;
    char *firstWord;
    symbolLink *lable;
    LINE *headOfFile;
    headOfFile = NULL;
    
    while(fgets(line, 81, filePointer))
    {
        strcpy(lineCopy, line);
        token = strtok(line, ":\t ");
        if((lable = findSymbol(headOfTable, token)) != NULL){
            char *tokenCopy = NULL;
            lable->adress = IC;
            token = strtok(NULL, "\n");
            strcpy(tokenCopy, token);
            firstWord = strtok(token, CUT);
            if(isACommand(cutWhiteChars(firstWord)))
                if(IC == 100)
                    headOfFile = toBinaryCommand(tokenCopy, headOfTable);
                else
                    addLine(headOfFile, toBinaryCommand(tokenCopy, headOfTable));
            else
                if(IC == 100)
                    headOfFile = toBinaryGuidance(tokenCopy);
                else
                    addLine(headOfFile, toBinaryGuidance(tokenCopy));
        }else if(!strcmp(token, ".extern")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 2;
        }else if(!strcmp(token, ".entry")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 1;
        } else if(isACommand(cutWhiteChars(token)))
            if(IC == 100)
                headOfFile = toBinaryCommand(lineCopy, headOfTable);
            else
                addLine(headOfFile, toBinaryCommand(lineCopy, headOfTable));
        else
            if(IC == 100)
                    headOfFile = toBinaryGuidance(lineCopy);
                else
                    addLine(headOfFile, toBinaryGuidance(lineCopy));
    }
    return headOfFile;
}


LINE *toBinaryGuidance(char line[])
{
    char *guidWord, *param;
    WORD *headForLine;
    LINE *node = NULL;
    node->wordHead = NULL;
    guidWord = strtok(line, " \t");
    if(!strcmp(guidWord, ".string"))
    {
        int i;
        param = strtok(NULL, "\"");
        param = strtok(NULL, "\"");
        headForLine = charToBinary(*(param));
        node->wordHead = headForLine;
        for(i = 1; i < strlen(param); i++)
            addWord(headForLine, charToBinary(*(param + i)));
    }else{
        int count = 1;
        while(param != NULL)
        {
            int *paramInBinary, i, num, k = 1;
            WORD *link = NULL;
            param = strtok(NULL, ",");
            for(i = strlen(param)-1; i >= 0; i--)
            {
                if(*(param + i) == '-')
                    num = num * (-1);
                else if(isdigit(*param + i)){
                    num = num + (*(param + i) * k);
                    k = k * 10;
                }
            }
            paramInBinary = decToBinary(num);
            link->word[18] = 1;
            for(i = 0; i < 16; i++)
                link->word[i] = *(paramInBinary + i);
            if(count)
                headForLine = link;
            else
                addWord(headForLine, link);
            count++;
            DC++;
        }
    }
    return node;
}

  

WORD *extraWordsToBinary(char *param)
{
    WORD *link = NULL;
    if(isNum(param))
    {
        int *numInBinary, i , num, k = 1;
        for(i = strlen(param)-1; i >= 0; i--)
        {
                if(*(param + i) == '-')
                    num = num * (-1);
                else{
                    num = num + (*(param + i) * k);
                    k = k * 10;
                }
        }
        numInBinary = decToBinary(num);
        for(i = 0; i < 16; i++)
            link->word[i] = *(numInBinary + i);
        link->word[18] = 1;
        IC++;
        return link;
    }else
        IC = IC + 2;
    return NULL;
}

WORD *charToBinary(char ch)
{
    WORD *link = NULL;
    int charInAscii = ch, *charInBinary, i;
    link->word[18] = 1;
    charInBinary = decToBinary(charInAscii);
    for(i = 0; i < 16; i++)
        link->word[i] = *(charInBinary);
    DC++;
    return link;

}

void addLine(LINE *head, LINE *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
    link->next = NULL;
}

void addWord(WORD *head, WORD *link)
{
    while(head->next != NULL)
        head = head->next;

    head->next = link;
    link->next = NULL;
}

int isNum(char *str)
{
    if(*str == '#')
        return 1;
    return 0;
}

int extractRegister(char * param)
{
    char *token;
    token = strtok(param, "[");
    token = strtok(NULL, "]");
    return isARegister(token);
} 

int isARegister(char line [])
{
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

int *decToBinary(int num){
    int *array, i;
    array = calloc(16, sizeof(int));

    if( num >= 0 ){
        for(i = 0 ; i < 17 ; i ++){    
            array[i] = num%2;    
            num = num/2;    
        }  
        return array;
    }

    num = (~num);

    for(i = 0 ; i < 17 ; i ++){    
        array[i] = num%2;    
        num = num/2;    
    }

    for(i = 0 ; i < 17 ; i ++){    
        if( array[i] == 1 )
            array[i] = 0;
        else if( !array[i] )
            array[i] = 1;
    }  

    return array;
}
