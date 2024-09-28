/********************************************************************************
 * File Name: scan.c                                                            *
 * Description: This file contains all the logic for the lexical analysis of    *
 *    the input.                                                                *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Return the position of character c in string s, or -1 if not found
static int chr_pos(const char *str, int c)
{
    const char *i;

    i = strchr(str, c);
    return (i ? i - str : -1);
}

// Putback a character that we don't need to deal with yet, return the old character
static int putback(int c)
{
    int old;

    old = Putback;
    Putback = c;
    if (c == 0)
    {
        Length = 0;
    }
    else
    {
        Length--;
    }
    return old;
}

// Get the next character from the input
static int next()
{
    int c;

    // Check if we have a character to put back
    if (Putback != 0)
    {
        c = putback(0); // Get from putback
    }
    else
    {
        c = fgetc(Input); // Read from the input file
    }
    Length++;
    return c;
}

// Skip any input that we don't need to deal with
static int skip()
{
    int c;

    // Get the next character
    c = next();
    // Skip whitespace, newlines, tabs, comments, etc...
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '#')
    {
        if (c == '#')
        {
            // Skip the comment
            while ((c = next()) != EOF && c != '\n')
                ;
        }
        // Check for newline to update line and column numbers
        if (c == '\n')
        {
            Line++;
            Column = 0;
        }
        Column++;
        Length = 0;
        // Fetch the next character
        c = next();
    }
    return c;
}

// Scan an integer from the input
static int scan_int(int c)
{
    int k, val = 0;

    // Convert each character into an int value
    while ((k = chr_pos("0123456789", c)) != -1)
    {
        val = val * 10 + k;
        c = next();
    }
    // We hit a character that isn't a digit, put it back
    putback(c);
    return val;
}

// Scan the next token from the input, and put it in the Token structure
void scan()
{
    int c;

    // Update the column number and reset the token length
    Column += Length;
    Length = 0;
    // Skip whitespace, newlines, tabs, etc...
    c = skip();
    // Determine the token based on the input
    switch (c)
    {
    case EOF:
        Token.type = T_EOF;
        Token.value = NO_VALUE;
        break;
    case '+':
        Token.type = T_PLUS;
        Token.value = NO_VALUE;
        break;
    case '-':
        Token.type = T_MINUS;
        Token.value = NO_VALUE;
        break;
    case '*':
        Token.type = T_STAR;
        Token.value = NO_VALUE;
        break;
    case '/':
        Token.type = T_SLASH;
        Token.value = NO_VALUE;
        break;
    case '%':
        Token.type = T_PERCENT;
        Token.value = NO_VALUE;
        break;
    default:
        // If it's a digit, scan the integer literal
        if (isdigit(c))
        {
            Token.type = T_INTLIT;
            Token.value.integer = scan_int(c);
            break;
        }
        // Otherwise, it's an error
        unrecognized_char_error(c);
    }
}

// Match the expected token, and scan the next token
VALUE match(TOKEN_TYPE typ)
{
    char buff[24];
    VALUE val;

    val = Token.value;
    if (typ != Token.type)
    {
        sprintf(buff, "'%s'", TOKEN_STR[typ]);
        match_error(buff, TOKEN_STR[Token.type]);
    }
    scan();
    return val;
}
