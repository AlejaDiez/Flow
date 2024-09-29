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

// Parse a print statement
static AST *print()
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
    case T_INTLIT:
        return expression();
    case T_PRINT:
        return print();
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
