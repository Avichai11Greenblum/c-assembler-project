#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void first_pass(FILE *filePointer, string fileName, int fileLength)
{
    char **macroDetails;
    char line[82];
    char *word;
    int inmacro = 0, i = 0, j = 0, macroCounter = 0, linesInMacro = 1;
    FILE *obFile = fopen(strcat(fileName.data, ".ob"), 'w');
    macroDetails = calloc(7 * (fileLength/2) ,sizeof(char *));
    while (fgets(line, 82, filePointer))
    {
        if(line[i] == '}')
            i++;
        i = move_to_none_white(line, i);
        for(; !isspace(line[i]); j++, i++)
                word[j] = line[i];
        if(!inmacro)
        {
            if (!strcmp(word, "macro"))
            {
                inmacro = 1;
                i = move_to_none_white(line, ++i);
                free(word);
                for(j = 0; !isspace(line[i]); i++, j++)
                    word[j] = line[i];
                *(macroDetails + macroCounter) = word;
            }
            else
                fputs(line, obFile);
        } else
        {
            if (!strcmp(word, "endm"))
            {
                inmacro = 0;
                macroCounter++;
                break;
            }
            else{
                *(macroDetails + macroCounter +(linesInMacro * (fileLength/2))) = line;
                linesInMacro++;
            }
        }
        free(word);
    }
    i = 0;
    while(*(macroDetails + i) != '\0')
    {
        int k = 0, l = 0;
        for(; k < fileLength; k++)
        {
            l = move_to_none_white(line, l);
            while(line[l] != '\n')
            {
                for(j = 0; !isspace(line[l]); j++, l++)
                    word[j] = line[l];
            }
        }
    }
}

