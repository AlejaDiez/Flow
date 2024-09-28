/********************************************************************************
 * File Name: data.h                                                            *
 * Description: This file contains all the global variables used.               *
 ********************************************************************************/

#ifndef extern_
#define extern_ extern
#endif

extern_ FILE *Input; // Input file
extern_ char *Name;  // Name of the input file

extern_ int Putback; // Character that was put back
extern_ int Line;    // Current line number
extern_ int Column;  // Column at which token starts
extern_ int Length;  // Length of current token
extern_ TOKEN Token; // Current token
