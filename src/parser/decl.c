/********************************************************************************
 * File Name: decl.c                                                            *
 * Description: This file contains all the logic for parsing declarations.      *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Parse a variable declaration
void var_declaration()
{
    VALUE ident;

    // Match the sintax
    match(T_VAR);
    // Add the variable to the global symbol table
    ident = match(T_IDENT);
    add_glob(ident.string);
    gen_global(ident.string);
    // Match the sintax
    match(T_COLON);
    match(T_INT);
    // Clean up
    free(ident.string);
}
