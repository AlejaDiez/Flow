/********************************************************************************
 * File Name: defs.h                                                            *
 * Description: This file contains all the definitions used.                    *
 ********************************************************************************/

#include <stdio.h>
#include <sysexits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256
#define NO_VALUE \
    (VALUE) { .pointer = NULL }

// Values
typedef union VALUE
{
    int integer;
    void *pointer;
} VALUE;

// Token
typedef enum TOKEN_TYPE
{
    T_EOF,
    // Operators
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_PERCENT,
    // Literals and Identifiers
    T_INTLIT
} TOKEN_TYPE;

static const char *const TOKEN_STR[] = {
    "eof",
    // Operators
    "+",
    "-",
    "*",
    "/",
    "%",
    // Literals and Identifiers
    "integer"};

typedef struct TOKEN
{
    TOKEN_TYPE type;
    VALUE value;
} TOKEN;

// Abstract Syntax Tree
typedef enum AST_TYPE
{
    // Literals and Identifiers
    A_INTLIT,
    // Arithmetic Operators
    A_ADD,
    A_SUB,
    A_MUL,
    A_DIV,
    A_MOD
} AST_TYPE;

typedef struct AST
{
    AST_TYPE type;
    struct AST *left;
    struct AST *right;
    VALUE value;
} AST;
