# C Project: 20-bit Assembler

## About
This project is an assignment in a C programming course at the open unvieristy (20465). The purpose of this project is to build an assembler for 15-instruction asssembly language, for an imaginary 20-bit CPU memory size.


## Source structure (Modules)
1. `main` - main function definition, argv & argc processing, single file processing.
2. `macro.c` + `macro.c` - contains functions for processing a single line in the first pass and inserts the macros into a new ob file text.
3. `list.c` + `list.h` - contains functions for custom made struct LIST and implementations of it.
4. `makeValid.c` + `makeValid.h` - contains functions for processing a single line and checkeing if the text follows the rules of compiler with feedback.
5. `symbolTable.c` + `symbolTable.h` - contains functions for custom made struct symbolLink and implementations of it.
6. `binary.c` + `binary.h` -  contains functions for processing lines and converting them to binary code according to the manual.
7. `toSpecial.c` - contains functions for converting binary code to hexa-like base that is required in the project.
8. `utils` - contains general-purposed, project-wide functions.
9. `makefile` - contains the recipe for compilation.

## Usage
`./assembler file1 file2 file3 ...`
Will start the processing of each file (specify WITHOUT extension!). No support for double-quoting file names.

## Build
You may build with several tools:
1. Using `make` command (Makefile)
2. Manually (Not recommended): compile each `*.c` file to `*.o` and then compile all to a single executable using `CC`/`GCC`/`CLang` with `-g a.o b.o .... -o exe`
