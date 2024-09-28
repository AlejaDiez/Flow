/********************************************************************************
 * File Name: decl.h                                                            *
 * Description: This file contains all the prototypes for the functions used.   *
 ********************************************************************************/

// scanner/scan.c
void scan();

// utils/err.c
void fatal(const char *msg, int cod);
void syntax_error(const char *msg);
void unrecognized_char_error(char chr);
