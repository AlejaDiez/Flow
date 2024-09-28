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

// utils/tree.c
AST *make_AST_node(AST_TYPE typ, AST *lft, AST *rgt, VALUE val);
AST *make_AST_binary(AST_TYPE typ, AST *lft, AST *rgt, VALUE val);
AST *make_AST_unary(AST_TYPE typ, AST *chld, VALUE val);
AST *make_AST_leaf(AST_TYPE typ, VALUE val);
