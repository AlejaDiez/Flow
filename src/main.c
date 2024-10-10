/********************************************************************************
 * File Name: main.c                                                            *
 *                                                                              *
 * Description: This file contains the main entry point for the Flow Compiler,  *
 *    a new programming language that is designed to be simple and easy to use. *
 * Author: Alejandro Diez Bermejo                                               *
 * Date: 2024-09-28                                                             *
 * Version: 0.0.0-dev                                                           *
 *                                                                              *
 * Compilation:                                                                 *
 *    make clean                                                                *
 *    make                                                                      *
 *                                                                              *
 * Execution:                                                                   *
 *    ./bin/flow [args...] <file>                                               *
 ********************************************************************************/

#include "defs.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"

#define VERSION "0.0.0-dev"

// Initialize compiler
void init()
{
    Putback = 0;
    Line = 1;
    Column = 1;
    Length = 0;
    // Scan the first token
    scan();
}

// Clean up the compiler
void end()
{
    free(Name);
    fclose(Input);
    fclose(Output);
}

// Main entry point
int main(int argc, char *argv[])
{
    char *name, *ext;

    // Check if we have an input file
    if (argc < 2)
        fatal("no input files", EX_USAGE);
    // Input file
    Name = strdup(argv[1]);
    Input = fopen(Name, "r");
    if (Input == NULL)
        fatal("cannot open input file", EX_NOINPUT);
    ext = strrchr(Name, '.');
    if (ext == NULL || strcmp(ext, ".flow") != 0)
        fatal("input file must have a '.flow' extension", EX_USAGE);
    // Output file
    name = (char *)malloc((strlen(Name) - 2) * sizeof(char));
    strncpy(name, Name, ext - Name);
    name[ext - Name] = '\0';
    strcat(name, ".s");
    Output = fopen(name, "w");
    if (Output == NULL)
        fatal("unable to create output file", EX_OSERR);
    free(name);
    // Compile the input file
    init();
    gen_code();
    end();
    return 0;
}
