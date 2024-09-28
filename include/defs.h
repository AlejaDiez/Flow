/********************************************************************************
 * File Name: defs.h                                                            *
 * Description: This file contains all the definitions used.                    *
 ********************************************************************************/

#include <stdio.h>
#include <sysexits.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.0-dev"

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
