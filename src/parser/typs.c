/********************************************************************************
 * File Name: typs.c                                                            *
 * Description: This file contains all the logic for checking types.            *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Operator type to AST type
PRIMITIVE_TYPE tok_to_prim(TOKEN_TYPE typ)
{
    switch (typ)
    {
    case T_VOID:
        return P_VOID;
    case T_BOOL:
        return P_BOOL;
    case T_INT:
        return P_INT;
    default:
        match_error("primitive type", TOKEN_STR[typ]);
        return NO_PRIMITIVE;
    }
}

// Operator type to primitive type
PRIMITIVE_TYPE op_to_prim(AST_TYPE typ)
{
    switch (typ)
    {
    case A_ADD:
    case A_SUB:
    case A_MUL:
    case A_DIV:
    case A_MOD:
        return P_INT;
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_LE:
    case A_GT:
    case A_GE:
        return P_BOOL;
    default:
        match_error("an operator", "other expression");
        return -1;
    }
}

// Check if two types are compatible.
void match_primitive(PRIMITIVE_TYPE lft, PRIMITIVE_TYPE rgt)
{
    // Check if either type is void, which is not allowed
    if (lft == P_VOID || rgt == P_VOID)
    {
        syntax_error("void type in expression");
    }

    // If both types aren't the same, they are incompatible
    if (lft != rgt)
    {
        syntax_error("incompatible types in expression");
    }
}

// Check if two types are compatible and check if the operation is valid
void match_primitive_op(PRIMITIVE_TYPE lft, PRIMITIVE_TYPE rgt, AST_TYPE op)
{
    // Check if either type is void, which is not allowed
    if (lft == P_VOID || rgt == P_VOID)
    {
        syntax_error("void type in expression");
    }

    // If both types aren't the same, they are incompatible
    if (lft != rgt)
    {
        syntax_error("incompatible types in expression");
    }

    // If the operation is a comparison, the types must be integers
    if (lft == P_BOOL && (op != A_EQ && op != A_NE))
    {
        syntax_error("boolean type in arithmetic expression");
    }
}
