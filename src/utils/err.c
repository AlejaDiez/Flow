/********************************************************************************
 * File Name: err.c                                                             *
 * Description: This file contains all the error handling functions.            *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Print the line with the error position
static void line_error()
{
    char *buff;
    size_t len;
    long pos;

    // Reserve memory for the line
    len = MAX_LEN * sizeof(char);
    buff = (char *)malloc(len);
    // Get the current file position
    pos = ftell(Input);
    // Move to the beginning of the file
    fseek(Input, 0, SEEK_SET);
    // Get the line where the error occurred
    for (int i = 0; i < Line; i++)
    {
        if (fgets(buff, len, Input) == NULL)
        {
            return;
        }
    }
    // Check if the line has a newline character
    if (strchr(buff, '\n') == NULL)
    {
        strcat(buff, "\n");
    }
    // Print the line
    printf("\n%s", buff);
    // Print the error position
    for (int i = 1; i <= strlen(buff); i++)
    {
        if (i >= Column && i < Column + Length)
        {
            if (Length > 1 && i != Column)
            {
                printf("\x1b[31m~\x1b[0m");
            }
            else
            {
                printf("\x1b[31m^\x1b[0m");
            }
        }
        else
        {
            printf(" ");
        }
    }
    printf("\n");
    // Reset the file position
    fseek(Input, pos, SEEK_SET);
    // Free the memory
    free(buff);
}

// Print an error and exit
static void error(const char *typ, const char *msg, bool line, int cod)
{
    if (line)
    {
        line_error();
    }
    printf("\x1b[31m%s:\x1b[0m %s", typ, msg);
    if (line)
    {
        printf(" (%s:%d:%d)", InputName, Line, Column);
    }
    printf("\n");
    free(InputName);
    fclose(Input);
    fclose(Output);
    remove(OutputName);
    free(OutputName);
    exit(cod);
}

// Print an error message and exit
void fatal(const char *msg, int cod)
{
    error("Error", msg, false, cod);
}

// Print a syntax error message and exit
void syntax_error(const char *msg)
{
    error("Syntax Error", msg, true, EX_DATAERR);
}

// Print an error message for an unrecognized character and exit
void unrecognized_char_error(char chr)
{
    char buff[128];

    sprintf(buff, "unrecognized character '%c'", chr);
    syntax_error(buff);
}

// Print an error message for an unrecognized token and exit
void unrecognized_token_error()
{
    char buff[128];

    sprintf(buff, "unexpected token '%s'", TOKEN_STR[Token.type]);
    syntax_error(buff);
}

// Print a match error message and exit
void match_error(const char *exptd, const char *fnd)
{
    char buff[128];

    sprintf(buff, "expected %s, but found '%s'", exptd, fnd);
    syntax_error(buff);
}

// Print an undeclared variable error message and exit
void undeclared_variable_error(const char *ident)
{
    char buff[128];

    snprintf(buff, sizeof(buff), "undeclared variable '%s'", ident);
    syntax_error(buff);
}

// Print a compiler error message and exit
void compile_error(const char *msg)
{
    error("Compile Error", msg, false, EX_SOFTWARE);
}
