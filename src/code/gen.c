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

// Load standard librearies functions
static void gen_load_lib()
{
    x86_64_load_lib();
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

// Generate the assembly code for a global variable
void gen_global(const char *str)
{
    x86_64_global(str);
}

// Store a register value in a global variable and return the register number
static int gen_store_global(int reg, const char *str)
{
    return x86_64_store_global(reg, str);
}

// Load a global variable into a register and return the register number
static int gen_load_global(const char *str)
{
    return x86_64_load_global(str);
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

// Generate the assembly code for a comparison operation and return the register number
static int gen_cmp(int reg_1, int reg_2, const char *cmp)
{
    return x86_64_cmp(reg_1, reg_2, cmp);
}

// Generate the assembly code for a print statement
static void gen_print(int reg)
{
    x86_64_print(reg);
}

// Generate the assembly code for an abstract syntax tree and return the register number
static int gen_ast(AST *n)
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
    case A_SEQ:
        gen_ast(n->left);
        gen_free_regs();
        gen_ast(n->right);
        gen_free_regs();
        // Clean up the AST nodes
        free(n->left);
        n->left = NULL;
        free(n->right);
        n->right = NULL;
        return NO_REG;
    case A_ASSIGN:
        lft_reg = gen_ast(n->left);
        gen_store_global(lft_reg, GlobalSymbols[n->right->value.integer].name);
        gen_free_reg(lft_reg);
        // Clean up the AST nodes
        free(n->left);
        n->left = NULL;
        free(n->right);
        n->right = NULL;
        return NO_REG;
    default:
        break;
    }

    // Generate the left AST node
    if (n->left)
    {
        lft_reg = gen_ast(n->left);
        // Clean up the left register
        free(n->left);
        n->left = NULL;
    }
    // Generate the right AST node
    if (n->right)
    {
        rgt_reg = gen_ast(n->right);
        // Clean up the right register
        free(n->right);
        n->right = NULL;
    }

    // Handle the different AST node types
    switch (n->type)
    {
    case A_INTLIT:
        return gen_load_int(n->value.integer);
    case A_IDENT:
        return gen_load_global(GlobalSymbols[n->value.integer].name);
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
    case A_EQ:
        return gen_cmp(lft_reg, rgt_reg, "e");
    case A_NE:
        return gen_cmp(lft_reg, rgt_reg, "ne");
    case A_LT:
        return gen_cmp(lft_reg, rgt_reg, "l");
    case A_LE:
        return gen_cmp(lft_reg, rgt_reg, "le");
    case A_GT:
        return gen_cmp(lft_reg, rgt_reg, "g");
    case A_GE:
        return gen_cmp(lft_reg, rgt_reg, "ge");
    case A_PRINT:
        gen_print(lft_reg);
        return NO_REG;
    case A_PAREN:
        return lft_reg;
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
    gen_load_lib();
    gen_function_prologue("main");
    reg = gen_ast(n);
    gen_function_epilogue();
    free(n);
    return reg;
}
