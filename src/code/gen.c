/********************************************************************************
 * File Name: gen.c                                                             *
 * Description: This file contains all the generic code generation functions.   *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Free all registers
static void gen_free_regs()
{
    return x86_64_free_regs();
}

// Free a register
static void gen_free_reg(int reg)
{
    return x86_64_free_reg(reg);
}

// Allocate a register
static int gen_alloc_reg()
{
    return x86_64_alloc_reg();
}

// Generate the assembly code for a function prologue
static void gen_function_prologue(const char *str)
{
    x86_64_function_prologue(str);
}

// Generate the assembly code for a function epilogue
static void gen_function_epilogue()
{
    x86_64_function_epilogue();
}

// Load an integer literal value into a register and return the register number
static int gen_load_int(int val)
{
    return x86_64_load_int(val);
}

// Generate the assembly code for an additon operation and return the register number
static int gen_add(int reg_1, int reg_2)
{
    return x86_64_add(reg_1, reg_2);
}

// Generate the assembly code for a subtraction operation and return the register number
static int gen_sub(int reg_1, int reg_2)
{
    return x86_64_sub(reg_1, reg_2);
}

// Generate the assembly code for a multiplication operation and return the register number
static int gen_mul(int reg_1, int reg_2)
{
    return x86_64_mul(reg_1, reg_2);
}

// Generate the assembly code for a division operation and return the register number
static int gen_div(int reg_1, int reg_2)
{
    return x86_64_div(reg_1, reg_2);
}

// Generate the assembly code for a modulo operation and return the register number
static int gen_mod(int reg_1, int reg_2)
{
    return x86_64_mod(reg_1, reg_2);
}

// Generate the assembly code for an abstract syntax tree and return the register number
static int gen_AST(AST *n)
{
    // Register numbers
    int lft_reg, rgt_reg;

    // Handle NULL AST nodes
    if (n == NULL)
    {
        return NO_REG;
    }

    // Handle special cases
    switch (n->type)
    {
    default:
        break;
    }

    // Generate the left AST node
    if (n->left)
    {
        lft_reg = gen_AST(n->left);
        // Clean up the left register
        free(n->left);
        n->left = NULL;
    }
    // Generate the right AST node
    if (n->right)
    {
        rgt_reg = gen_AST(n->right);
        // Clean up the right register
        free(n->right);
        n->right = NULL;
    }

    // Handle the different AST node types
    switch (n->type)
    {
    case A_INTLIT:
        return gen_load_int(n->value.integer);
    case A_ADD:
        return gen_add(lft_reg, rgt_reg);
    case A_SUB:
        return gen_sub(lft_reg, rgt_reg);
    case A_MUL:
        return gen_mul(lft_reg, rgt_reg);
    case A_DIV:
        return gen_div(lft_reg, rgt_reg);
    case A_MOD:
        return gen_mod(lft_reg, rgt_reg);
    default:
        compile_error("unknown AST node type");
    }
    return NO_REG;
}

// Generate the assembly code for a program
int gen_code(AST *n)
{
    int reg;

    gen_free_regs();
    gen_function_prologue("main");
    reg = gen_AST(n);
    gen_function_epilogue();
    free(n);
    return reg;
}
