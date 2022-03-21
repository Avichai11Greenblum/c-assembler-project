run:	list.o	macros.o	symbolTable.o	toBinary.o	toSpecial.o	utils.o	validation.o	main.o
	gcc	-ansi	-Wall	-pedantic	list.o	macros.o	symbolTable.o	toBinary.o	toSpecial.o	utils.o	validation.o	main.o	-o	run
list.o:	list.c	list.h
	gcc	-c	-ansi	-Wall	-pedantic	list.c	-o	list.o
macros.o:	macros.c	macros.h	utils.h
	gcc	-c	-ansi	-Wall	-pedantic	macros.c	-o	macros.o
symbolTable.o:	symbolTable.c	symbolTable.h
	gcc	-c	-ansi	-Wall	-pedantic	symbolTable.c	-o	symbolTable.o
toBinary.o:	toBinary.c	toBinary.h
	gcc	-c	-ansi	-Wall	-pedantic	toBinary.c	-o	toBinary.o
toSpecial.o:	toSpecial.c
	gcc	-c	-ansi	-Wall	-pedantic	toSpecial.c	-o	toSpecial.o
utils.o:	utils.c	utils.h
	gcc	-c	-ansi	-Wall	-pedantic	utils.c	-o	utils.o
validation.o:	validation.c	validation.h
	gcc	-c	-ansi	-Wall	-pedantic	validation.c	-o	validation.o
main.o:	main.c	toBinary.h	macros.h
	gcc	-c	-ansi	-Wall	-pedantic	main.c	-o	main.o
