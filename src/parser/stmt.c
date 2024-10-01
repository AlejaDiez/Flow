/********************************************************************************
 * File Name: stmt.c                                                            *
 * Description: This file contains all the logic for parsing statements.        *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Add a statement to a sequence and return the new sequence, also check for a semi-colon
static AST *add_seq(AST *seq, AST *stmt)
{
    // Check if we have a statement
    if (!stmt)
    {
        return seq;
    }
    // Check if we have a semi-colon
    switch (stmt->type)
    {
    case A_SEQ:
        break;
    default:
        match(T_SEMICOLON);
    }
    // Check if we have a sequence
    if (!seq)
    {
        return stmt;
    }
    return make_AST_binary(A_SEQ, seq, stmt, NO_VALUE);
}

// Parse a variable assignment and return an AST
static AST *assignment_statement()
{
    AST *n, *lft, *rgt;
    VALUE ident;
    int id;

    // Match the sintax
    ident = match(T_IDENT);
    // Find the identifier in the global symbol table
    id = find_glob(ident.string);
    if (id == -1)
        undeclared_variable_error(ident.string);
    rgt = make_AST_leaf(A_IDENT, (VALUE){id});
    // Match the sintax
    match(T_ASSIGN);
    // Parse the expression
    lft = expression();
    // Create the AST
    n = make_AST_binary(A_ASSIGN, lft, rgt, NO_VALUE);
    return n;
}

// Parse a print statement
static AST *print_statement()
{
    AST *expr;

    // Match syntax
    match(T_PRINT);
    match(T_LPAREN);
    // Parse an expression
    expr = expression();
    // Match syntax
    match(T_RPAREN);
    return make_AST_unary(A_PRINT, expr, NO_VALUE);
}

// Parse a statement
AST *statement()
{
    switch (Token.type)
    {
    case T_LPAREN:
    case T_INTLIT:
        return expression();
    case T_IDENT:
        if (look_ahead().type == T_ASSIGN)
        {
            return assignment_statement();
        }
        return expression();
    case T_VAR:
        var_declaration();
        return NULL;
    case T_PRINT:
        return print_statement();
    default:
        unrecognized_token_error();
    }
    return NULL;
}

// Parse multiple statements
AST *sequence()
{
    AST *seq = NULL, *stmt = NULL;

    // Parse the statements
    while (Token.type != T_EOF)
    {
        stmt = statement();
        seq = add_seq(seq, stmt);
    }
    // Match the sintax
    match(T_EOF);
    return seq;
}
