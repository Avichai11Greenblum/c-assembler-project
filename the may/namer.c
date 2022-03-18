#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t ,\n"
#define binToHex_H

int IC = 100; /*holds the address of the word in the CPU*/
int DC = 0; /*holds the num of commands that are in the data image*/

/* custom made struct for our commands */
typedef struct commandsStruct{
    char commandName[31];
    int opcode;
    int funct;
    int numOfParam;     
}commandsStruct;

/*an array that holds all the info of all commands*/
commandsStruct ourCommands[] = {
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 10, 2},
    {"sub", 2, 11, 2},
    {"lea", 4, 0, 2},

    /* second group -> have 1 operand */
    {"clr", 5, 10, 1},
    {"not", 5, 11, 1},
    {"inc", 5, 12, 1},
    {"dec", 5, 13, 1},
    {"jmp", 9, 10, 1},
    {"bne", 9, 11, 1},
    {"jsr", 9, 12, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},

    /* third group -> no operands */
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}
};

/*an objest that holds the 20 bytes array for each word in line and a pointer to the next (implementation of linked list)*/
typedef struct WORD
{
    int word[20];
    struct WORD *next;
}WORD;

/*an object that holds all info of a lable and a pointer to the next(implementation of linked list)*/
typedef struct symbolLink
{
	char name[31];
	int adress;
	int ext; 
  int ent;
  int times;
	struct symbolLink *next;

}symbolLink;


/*funcs declarations*/
symbolLink * symboleTableCreat(FILE *filePointer);
symbolLink *addSymbol(symbolLink *head, char lableName[]);
struct symbolLink* findSymbol(struct symbolLink *head, char lableName[]);
char *cutWhiteChars(char *str);
WORD *firstPass(FILE *filePointer, symbolLink *headOfTable);
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile);
void toBinaryGuidance(char line[], WORD *headOfFile);
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable);
WORD *charToBinary(char ch);
void addWord(WORD *head, WORD *link);
int isNum(char *str);
int extractRegister(char * param);
int isARegister(char line []);
int *decToBinary(int num);
void zeroMe(int word[]);
void trimTrailing(char * str);
int isACommand(char line[] );
commandsStruct *findCommand(char * command);
void deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile);
int allZero(WORD *link);
void changeWord(WORD *headOfFile, symbolLink *symbolFound, FILE *entFile, FILE *extFile);
void secondPass(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable, char *fileName);
void binToSpecial (int num [],int address,FILE* output);
char binToHex(int num []);
void output(WORD *head);


/*a func to create the output file - in hex base*/
void output(WORD *head){
  int address = 100;
  FILE *output;
  WORD *ptr = head;
  output = fopen("output.ob","w");
  fprintf(output,"\t\t\t%d\t%d\n",IC-100-DC,DC);
  while(ptr != NULL && address <= IC){
    binToSpecial(ptr->word,address,output);
    ptr = ptr->next;
    address++;
  }
}

/*a func to get the num in he special base specified*/
void binToSpecial (int num [],int address,FILE* output){
  int num1 [] = {0,0,0,0};
  int num2 [] = {0,0,0,0};
  int num3 [] = {0,0,0,0};
  int num4 [] = {0,0,0,0};
  int num5 [] = {0,0,0,0};
  int i,j;
  char a, b, c, d, e;
  int count = 0;
  for(i=0; i<5; i++){
    for(j=0;j<4;j++){
      if(i == 0)
        num1[j] = num[count];
      else if(i == 1)
        num2[j] = num[count];
      else if(i == 2)
        num3[j] = num[count];
      else if(i == 3)
        num4[j] = num[count];
      else if(i == 4)
        num5[j] = num[count];
      count++;
    }
  }
  a = binToHex(num1);
  b = binToHex(num2);
  c = binToHex(num3);
  d = binToHex(num4);
  e = binToHex(num5);
  if(address < 1000)
    fprintf(output, "0%d\tA%c-B%c-C%c-D%c-E%c\n", address, e, d, c, b, a);
  else{
    fprintf(output, "%d\tA%c-B%c-C%c-D%c-E%c\n", address, e, d, c, b, a);
  }
}

/*a func to convert a num in binary to hex base*/
char binToHex(int num []){
  if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 0) 
    return '0';
  if(num[3] == 0 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '1';
  if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return '2';
  if(num[3] == 0 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return '3';
  if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return '4';
  if(num[3] == 0 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return '5';
  if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return '6';
  if(num[3] == 0 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return '7';
  if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 0)
    return '8';
  if(num[3] == 1 && num[2] == 0 && num[1] == 0 && num[0] == 1)
    return '9';
  if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 0)
    return 'a';
  if(num[3] == 1 && num[2] == 0 && num[1] == 1 && num[0] == 1)
    return 'b';
  if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 0)
    return 'c';
  if(num[3] == 1 && num[2] == 1 && num[1] == 0 && num[0] == 1)
    return 'd';
  if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 0)
    return 'e';
  if(num[3] == 1 && num[2] == 1 && num[1] == 1 && num[0] == 1)
    return 'f';
  return '\0';
}


/* func for initializing the 20 bits in a word to zeros */
void zeroMe(int word[])
{
  int i;
  for(i = 0; i < 20; i++)
    word[i] = 0;
}

/*a func to trim the white chars in the end of a string*/
void trimTrailing(char * str){
    int index = -1;
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }
    str[index + 1] = '\0';
}

/*a func to create a linked list from all the lables in the file*/
symbolLink *symboleTableCreat(FILE *filePointer)
{
    char line[81];
    char *token;
    symbolLink *head = (struct symbolLink*)malloc(sizeof(struct symbolLink));
    symbolLink *lableFound = NULL;
    head->next = NULL;
    while (fgets(line, 81, filePointer))
    {       
        token = strtok(line, CUT);
        if(token[strlen(token)-1] == ':' && findSymbol(head, strtok(token, ":")) == NULL)
        {
            head = addSymbol(head, token);
        }
        else if(!strcmp(token, ".extern"))
        {
            token = strtok(NULL, CUT);
            head = addSymbol(head, token);
            head->ext = 1;
            head->ent = 0;
        } else if(!strcmp(token, ".entry"))
        {          
            token = strtok(NULL, CUT);
            lableFound = findSymbol(head, token);
            if(lableFound == NULL)
            {
                head = addSymbol(head, token);
                head->ent = 2;
            } else{
              lableFound->ent = 2;
            }
        }
    }
    return head;
}

/*a func to add a lable to the linked list*/
symbolLink *addSymbol(symbolLink *head, char lableName[])
{
    struct symbolLink *link = (struct symbolLink *) malloc (sizeof(struct symbolLink));
    strcpy(link->name, lableName);
    if( head == NULL ){
      link->next = NULL;
      return link;
    }
    link->ent = 1;
    link->times = 1;
    link->next = head;
    head = link;
    return head;
}

/*a func to find an object in the list- if found- returns the object - else - returns null*/
struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]) {
    struct symbolLink* current = head;
    if(head == NULL)
        return NULL;
    while(current != NULL)  
        if(!strcmp(current->name, lableName))
          return current;
        else
          current = current->next;
    return NULL;
}

/*a func to check if a string is one of the commands*/
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

/*a func to get the command object that contains the command's info*/
commandsStruct *findCommand(char * command)
{
    int i = 0;
    for(; i < 16; i++)
    {
        if(!strcmp(command, ourCommands[i].commandName))
        	return &ourCommands[i];
    }
    return NULL;
}

/* func that distinguishes what address is given and codes it into binary */
void deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile)
{ 
    int isDest = 1;
    int i, j;
    char* token, lineCopy[81]; 
    WORD* link =(struct WORD*)malloc(sizeof(struct WORD)); 
    strcpy(lineCopy, myStr);
    token = strtok(myStr, ", \t\n");
    zeroMe(link->word);
    link->next = NULL;
    link->word[18] = 1;
    if(command->funct != 0)
    {
      int *functInBin;
      functInBin = decToBinary(command->funct);
      for(i = 12, j = 0; i < 16; i++, j++)
        link->word[i] = functInBin[j];
    }

    if(command->numOfParam != 0)
    {
      if(command->numOfParam == 2)
        isDest = 0;
      while(token != NULL)
      {
        /* Delivery 0 */ 
        if( token[0] == '#')
        {
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

        if(command->numOfParam == 2 && !isDest)
        {
          char temp[81];
          int i, j = 0, counter = 0;
          for(i = strlen(lineCopy)-1; i >=0 && lineCopy[i] != ',' && lineCopy[i] != ' '; i--)
            counter++;
          for(i = strlen(lineCopy) - counter; i <strlen(lineCopy) ;i++, j++)  
            temp[j] = lineCopy[i];
          token = temp;
          *(token + j) = '\0';
        }
        else
          token = NULL;
        isDest = 1;
      }
      addWord(headOfFile, link);
    }
}

/* func that removes white chars from a given string */
char *cutWhiteChars(char *str)
{
    char *newStr;
    int i, j = 0;
    newStr = malloc(strlen(str) * sizeof(char));
    for(i = 0; i < strlen(str); i++)
    {
        if(!isspace(*(str + i)))
        {
            *(newStr + j) = *(str + i);
            j++;
        }
    }
    *(newStr + j) = '\0';
    return newStr;
}

/* func that receives a command sentence and codes it into binary code */
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile)
{
    char *command, *token, *restOfString;
    commandsStruct *commandFound = NULL;
    WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
    char lineCopy[81];
    char lineCopy2[81];
    restOfString = (char*)malloc(81);
    zeroMe(link->word);
    link->next = NULL;

    strcpy(lineCopy, line);
    strcpy(lineCopy2, line);
    command = strtok(line, CUT);
    commandFound = findCommand(command);
    link->word[commandFound->opcode] = 1;
    link->word[18] = 1;
    addWord(headOfFile, link);
    *(restOfString + strlen(restOfString)) = '\n';
    if(commandFound->numOfParam != 0)
      restOfString = strtok(NULL, command);
    deliveryForBinary(commandFound, restOfString, headOfTable, headOfFile);
    token = strtok(restOfString, ",\n");
    if(token !=NULL &&(!(*(cutWhiteChars(token)) == 'r' && isARegister(cutWhiteChars(token)) != -1)))
          extraWordsToBinary(cutWhiteChars(token), headOfFile, headOfTable);
    if(commandFound->numOfParam == 2)
    {
      char temp[81];
      int i, j = 0, counter = 0;
      for(i = strlen(lineCopy2)-1; i >=0 && lineCopy2[i] != ',' && lineCopy2[i] != ' '; i--)
        counter++;
      for(i = strlen(lineCopy2) - counter; i <strlen(lineCopy2) ;i++, j++)  
        temp[j] = lineCopy2[i];
      if(!(*(cutWhiteChars(temp)) == 'r' && isARegister(cutWhiteChars(temp)) != -1))
          extraWordsToBinary(cutWhiteChars(temp), headOfFile, headOfTable);
    }
}

/* func that goes over the given input and codes it into binary excluding lables */
WORD *firstPass(FILE *filePointer, symbolLink *headOfTable)
{
    char line[81];
    char lineCopy[81];
    char *token;
    char *firstWord;
    symbolLink *lable;
    WORD *headOfFile = (struct WORD*)malloc(sizeof(struct WORD));
    headOfFile->next = NULL;
    
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

/* func that receives a instruction sentence and codes it into binary code */
void toBinaryGuidance(char line[], WORD *headOfFile)
{
    char *guidWord, *param = NULL;
    guidWord = strtok(line, " \t");
    if(!strcmp(guidWord, ".string"))
    {
      int i;
      param = strtok(NULL, "\"");
      for(i = 0; i < strlen(param); i++)
      {
        addWord(headOfFile, charToBinary(*(param + i)));
        DC++;
      }
      addWord(headOfFile, charToBinary('\0'));
      DC++;
      
    }else{
      param = strtok(NULL, ",");
      while( param != NULL)
      {
        WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
        int *paramInBinary, i, num = 0, k = 1;
        trimTrailing(param);
        zeroMe(link->word);
        for(i = strlen(param)-1; i >= 0; i--)
        {
          if(param[i] == '-')
              num = num * (-1);

          else if( isdigit(param[i]) ){
            num += (param[i] - 48) * k;
            k *= 10;
          }
        }
        paramInBinary = decToBinary(num);
        link->word[18] = 1;
        for(i = 0; i < 16; i++)
          link->word[i] = *(paramInBinary + i);
        addWord(headOfFile, link);
        DC++;
        param = strtok(NULL, ", \n");
      }
    }
}

/* func that codes each operand to binary based on the given addressing and line type */
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable)
{
    WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
    zeroMe(link->word);
    link->next = NULL;

    if(isNum(param))
    {
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
      numInBinary = decToBinary(num);
      for(i = 0; i < 16; i++)
        link->word[i] = *(numInBinary + i);
      link->word[18] = 1;
      addWord(headOfFile, link);
    }else if(findSymbol(headOfTable,cutWhiteChars(param)) != NULL)
    {
      WORD *link1 = (struct WORD*)malloc(sizeof(struct WORD));
      WORD *link2 = (struct WORD*)malloc(sizeof(struct WORD));
      zeroMe(link1->word);
      zeroMe(link2->word);
      addWord(headOfFile, link1);
      addWord(headOfFile, link2);
    }
}

/* func that converts a char to the first 16 out of the 20 bits */
WORD *charToBinary(char ch)
{
  WORD *link =(struct WORD*)malloc(sizeof(struct WORD));
  int charInAscii = ch, *charInBinary, i;
  zeroMe(link->word);
  link->word[18] = 1;
  charInBinary = decToBinary(charInAscii);
  for(i = 0; i < 16; i++)
      link->word[i] = *(charInBinary + i);
  link->next = NULL;
  return link;
}

/* func that adds a WORD type struct to a given linked list */
void addWord(WORD *head, WORD *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
    link->next = NULL;
    IC++;
}

/* func that checks if a given operand in string type is a number or not */
int isNum(char *str)
{
    if(str[0] == '#')
      return 1;
    return 0;
}

/* func that receives a string that has a register in it and extracts it's number out */
int extractRegister(char * param)
{
    char *token;
    token = strtok(param, "[");
    token = strtok(NULL, "]");
    return isARegister(token);
} 

/* func that checks if a given string is one of the valid registers and if so returns it's number */
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

/* func that converts a given int number to the required binary format */
int *decToBinary(int num){
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

/* boolean func that checks if all the 20 bits of a WORD have the value 0 */
int allZero(WORD *link)
{
  int i;
  for( i = 0; i < 20; i++){
    if(link->word[i] != 0)
      return 0;
  }
  return 1;
}

/* func that changes a word */
void changeWord(WORD *headOfFile, symbolLink *symbolFound, FILE *entFile, FILE *extFile)
{
  int i, counter = 100;
  WORD *current = headOfFile->next;
  while(current !=NULL)
  {
    if(allZero(current))
    {
      if(symbolFound->ext == 1)
      {
        current->word[16] = 1;
        current->next->word[16] = 1;
        if(symbolFound->times == 1)
        {
          fprintf(extFile, "%s\tBASE\t%d\n", symbolFound->name, counter);
          fprintf(extFile, "%s\tOFFSET\t%d\n\n", symbolFound->name, counter + 1);
          symbolFound->times++;
        }
      } else if(symbolFound->ent == 1 || symbolFound->ent == 2)
      {
        int offset = symbolFound->adress % 16;
        int base = symbolFound->adress - offset;
        int *offsetInBin, *baseInBin;
        current->word[17] = 1;
        current->next->word[17] = 1;
        baseInBin = decToBinary(base);
        for(i = 0; i < 16; i++)
          current->word[i] = *(baseInBin + i);
        offsetInBin = decToBinary(offset);
        for(i = 0; i < 16; i++)
          current->next->word[i] = *(offsetInBin + i);
      }
      break;
    }
    counter++;
    current = current->next;
  }
}

/* func that goes over the input for the second time and adds the needed extra words for all the labels and creates the entry and extern files */
void secondPass(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable, char *fileName)
{
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
  while(fgets(line, 81, filePointer))
  {
    token = strtok(line, ": \t,");
    if(findSymbol(headOfTable, token) != NULL)
      token = strtok(NULL, " \t,");
    if(isACommand(cutWhiteChars(token)))
    {
      cmnd = findCommand(cutWhiteChars(token));
      numOfParsLeft = cmnd->numOfParam;
      while(numOfParsLeft > 0)
      {
        token = strtok(NULL, "[ \t,");
        if((symbolFound = findSymbol(headOfTable, cutWhiteChars(token))) != NULL)
          changeWord(headOfFile, symbolFound, entFile, extFile);
        numOfParsLeft--;
      }
    }
  }
  while(current != NULL)
  {
    if(current->ent == 2)
    {
      int offset = current->adress % 16;
      fprintf(entFile, "%s,%d,%d\n", current->name, current->adress - offset, offset);
    }
    current = current->next;
  }
}

/* func that free the memory stormage from a given WORD */
void freeList(WORD *head)
{
  WORD *current = head;
  WORD *nextOne = head->next;

  while(current != NULL)
  {
    free(current);
    current = nextOne;
    if( current != NULL)
      nextOne = current->next;
  }
}

/* func that free the memory stormage from a given LIST */
void freeList2(symbolLink *head)
{
  symbolLink *current = head;
  symbolLink *nextOne = head->next;

  while(current != NULL)
  {
    free(current);
    current = nextOne;
    if( current != NULL)
      nextOne = current->next;
  }
}

/* a func that we do whatever we want in it */
int main(){

    WORD *headOfFile = NULL;
    FILE *fptr;
    FILE *ff ,*fp;
    symbolLink *head;
    int k, j;
    int bla[20];
    int lines = 0;
    char fileName[10] = "t";
    fptr = fopen("t.txt", "r");

    head = symboleTableCreat(fptr);
    fclose(fptr);
    ff = fopen("t.txt", "r");
    
    headOfFile = firstPass( ff, head );
    fclose(ff);

    fp = fopen("t.txt", "r");

    secondPass(fp, headOfFile, head, fileName);
    headOfFile = headOfFile->next;
    output(headOfFile);
    for(k = 19, j = 0; k >= 0; k--, j++)
      bla[k] = j;
    printf("lines b-> ");
    for(k = 0; k < 20; k++)
      printf("%d\t", bla[k]);
    while(headOfFile != NULL)
    {
        int i;
        printf("\nline %d -> ", lines);
        for(i = 19; i >= 0; i--)
        {
            printf("%d\t",headOfFile->word[i]);
        }
        printf("\n");
        lines++;
        headOfFile = headOfFile->next;
    }
    return 1;
}
