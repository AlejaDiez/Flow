/********************************************************************************
 * File Name: tree.c                                                            *
 * Description: This file contains all the functions for the AST tree.          *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Make a new AST node
AST *make_AST_node(AST_TYPE typ, AST *lft, AST *mid, AST *rgt, VALUE val)
{
    AST *n;

    // Malloc a new AST node
    n = (AST *)malloc(sizeof(AST));
    if (n == NULL)
        fatal("out of memory", EX_SOFTWARE);
    // Copy all the field values and return it
    n->type = typ;
    n->left = lft;
    n->middle = mid;
    n->right = rgt;
    n->value = val;
    return n;
}

// Make a new AST binary node
AST *make_AST_binary(AST_TYPE typ, AST *lft, AST *rgt, VALUE val)
{
    return make_AST_node(typ, lft, NULL, rgt, val);
}

// Make a new AST unary node
AST *make_AST_unary(AST_TYPE typ, AST *chld, VALUE val)
{
    return make_AST_node(typ, chld, NULL, NULL, val);
}

// Make a new AST leaf node
AST *make_AST_leaf(AST_TYPE typ, VALUE val)
{
    return make_AST_node(typ, NULL, NULL, NULL, val);
}
