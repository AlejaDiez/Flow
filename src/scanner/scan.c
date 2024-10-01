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

// Scan an identifier
static char *scan_ident(int c)
{
    char *buff, *str;
    int i;

    buff = (char *)malloc(MAX_LEN * sizeof(char));
    // Check if we ran out of memory
    if (buff == NULL)
        fatal("out of memory", EX_SOFTWARE);
    i = 0;
    // Scan until we hit a character that isn't a letter, digit or underscore
    while (isalpha(c) || isdigit(c) || c == '_')
    {
        if (i < MAX_LEN - 1)
        {
            buff[i++] = c;
        }
        c = next();
    }
    // Put back the character that stopped the loop
    putback(c);
    // Check if the identifier is too long
    if (i == MAX_LEN - 1)
        syntax_error("identifier too long");
    buff[i] = '\0';
    // Copy the buffer into a new string
    str = strdup(buff);
    // Clean up the buffer
    free(buff);
    return str;
}

// Check if the identifier is a keyword and return the keyword token
static int keyword(const char *str)
{
    switch (*str)
    {
    case 'i':
        if (!strcmp(str, "int"))
            return T_INT;
        break;
    case 'p':
        if (!strcmp(str, "print"))
            return T_PRINT;
        break;
    case 'v':
        if (!strcmp(str, "var"))
            return T_VAR;
        break;
    }
    return T_IDENT;
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
    case '=':
        Token.type = T_ASSIGN;
        Token.value = NO_VALUE;
        break;
    case ':':
        Token.type = T_COLON;
        Token.value = NO_VALUE;
        break;
    case ';':
        Token.type = T_SEMICOLON;
        Token.value = NO_VALUE;
        break;
    case '(':
        Token.type = T_LPAREN;
        Token.value = NO_VALUE;
        break;
    case ')':
        Token.type = T_RPAREN;
        Token.value = NO_VALUE;
        break;
    default:
        // If it's a digit, scan the integer literal
        if (isdigit(c))
        {
            int num = scan_int(c);

            Token.type = T_INTLIT;
            Token.value.integer = num;
            break;
        }
        // If it's a letter or underscore, scan the identifier
        if (isalpha(c) || c == '_')
        {
            char *str;
            int type;

            str = scan_ident(c);
            type = keyword(str);
            Token.type = type;
            if (type == T_IDENT)
            {
                Token.value.string = str;
            }
            else
            {
                Token.value = NO_VALUE;
                free(str);
            }
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

// Look ahead in the input to see the next tokens without consuming them
TOKEN look_ahead()
{
    TOKEN tok;
    struct
    {
        long position;
        int putback;
        int line;
        int column;
        int length;
        struct TOKEN token;
    } state;

    // Save the current state
    state.position = ftell(Input);
    state.putback = Putback;
    state.line = Line;
    state.column = Column;
    state.length = Length;
    state.token = Token;
    // Scan the next token
    scan();
    tok = Token;
    // Restore the state
    fseek(Input, state.position, SEEK_SET);
    Putback = state.putback;
    Line = state.line;
    Column = state.column;
    Length = state.length;
    Token = state.token;
    // Return the token
    return tok;
}
