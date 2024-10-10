/********************************************************************************
 * File Name: decl.c                                                            *
 * Description: This file contains all the logic for parsing declarations.      *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Operator type to AST type
PRIMITIVE_TYPE tok_to_primitive(TOKEN_TYPE typ)
{
    switch (typ)
    {
    case T_VOID:
        return P_VOID;
    case T_BOOL:
        return P_BOOL;
    case T_INT:
        return P_INT;
    default:
        match_error("primitive type", TOKEN_STR[typ]);
        return NO_PRIMITIVE;
    }
}

// Parse a variable declaration
void var_declaration()
{
    int id, typ;
    VALUE ident;

    // Match the sintax
    match(T_VAR);
    ident = match(T_IDENT);
    // Match the sintax
    match(T_COLON);
    typ = tok_to_primitive(Token.type);
    if (typ == P_VOID)
        syntax_error("variable cannot be of type void");
    scan();
    // Add the variable to the global symbol table
    id = add_glob(S_VARIABLE, ident.string, typ);
    // Generate the assembly code
    gen_global(id);
    // Clean up
    free(ident.string);
}

// Parse a function declaration
void fun_declaration()
{
    int id, typ;
    VALUE ident;
    AST *stmt = NULL;

    // Match the sintax
    match(T_FUN);
    ident = match(T_IDENT);
    // Match the sintax
    match(T_LPAREN);
    match(T_RPAREN);
    // Check if the function has a return type
    if (Token.type == T_COLON)
    {
        match(T_COLON);
        typ = tok_to_primitive(Token.type);
        scan();
    }
    else
    {
        typ = P_VOID;
    }
    // Parse the function body
    stmt = block_statement();
    // Add the function to the global symbol table
    id = add_glob(S_FUNCTION, ident.string, typ);
    // Generate the assembly code
    gen_function(id, stmt);
    // Clean up
    free(ident.string);
    free_AST(stmt);
}
