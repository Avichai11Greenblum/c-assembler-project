run.exe: firstPass.o symbolTable.o
        gcc -ansi -Wall -pedantic firstPass.o symbolTable.o  -o run.exe 

firstPass.o: firstPass.c firstPass.h
        gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o

symbolTable.o:  symbolTable.c   firstPass.h
        gcc -c -ansi -Wall -pedantic symbolTable.c -o symbolTable.o 

