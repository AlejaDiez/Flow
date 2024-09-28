/********************************************************************************
 * File Name: err.c                                                             *
 * Description: This file contains all the error handling functions.            *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Print an error and exit
static void error(const char *typ, const char *msg, int cod)
{
    printf("\x1b[31m%s:\x1b[0m %s\n", typ, msg);
    exit(cod);
}

// Print an error message and exit
void fatal(const char *msg, int cod)
{
    error("Fatal Error", msg, cod);
}
