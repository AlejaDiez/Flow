/********************************************************************************
 * File Name: gen.c                                                             *
 * Description: This file contains all the generic code generation functions.   *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

static int gen_ast(AST *n, AST *prt, int lbl);

// Generate a label
static int label()
{
    static int lbl = 0;

    return ++lbl;
}

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

// Generate the assembly code for the main function prologue
static void gen_main_function_prologue()
{
    x86_64_main_function_prologue();
}

// Generate a label
static void gen_label(int lbl)
{
    x86_64_label(lbl);
}

// Generate a jump to a label
static void gen_jump(int lbl)
{
    x86_64_jump(lbl);
}

// Generate the assembly code for a global variable
void gen_global(int id)
{
    x86_64_global(id);
}

// Store a register value in a global variable and return the register number
static int gen_store_global(int reg, int id)
{
    return x86_64_store_global(reg, id);
}

// Load a global variable into a register and return the register number
static int gen_load_global(int id)
{
    return x86_64_load_global(id);
}

// Generate the assembly code for a function prologue
static void gen_function_prologue(int id)
{
    x86_64_function_prologue(id);
}

// Generate the assembly code for a function epilogue
static void gen_function_epilogue()
{
    x86_64_function_epilogue();
}

// Generate the assembly code for a function
void gen_function(int id, AST *n)
{
    gen_function_prologue(id);
    gen_ast(n, NULL, NO_LABEL);
    gen_function_epilogue();
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

// Generate the assembly code for a comparison and jump operation and return the register number
static int gen_cmp_jump(int reg_1, int reg_2, const char *jmp, int lbl)
{
    return x86_64_cmp_jump(reg_1, reg_2, jmp, lbl);
}

// Generate the assembly code for a print statement
static void gen_print(int reg)
{
    x86_64_print(reg);
}

// Generate the assembly code for an abstract syntax tree and return the register number
static int gen_ast(AST *n, AST *prt, int lbl)
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
        // Generate the left AST node
        gen_ast(n->left, n, NO_LABEL);
        gen_free_regs();
        // Generate the right AST node
        gen_ast(n->right, n, NO_LABEL);
        gen_free_regs();
        // Clean up the AST nodes
        free_AST_chld(n);
        return NO_REG;
    case A_ASSIGN:
        // Generate the left AST node
        lft_reg = gen_ast(n->left, n, NO_LABEL);
        // Store the value in the global variable
        gen_store_global(lft_reg, n->right->value.integer);
        gen_free_reg(lft_reg);
        // Clean up the AST nodes
        free_AST_chld(n);
        return NO_REG;
    case A_IFELSE:
        // Generate false label (if there is no else clause, this is the end label)
        lft_reg = label();
        if (n->right)
        {
            rgt_reg = label(); // Generate the end label
        }
        // Generate the left AST node (condition)
        gen_ast(n->left, n, lft_reg);
        gen_free_regs();
        // Generate the mid AST node (true clause)
        gen_ast(n->middle, n, NO_LABEL);
        gen_free_regs();
        // Generate the end label jump if there is an else clause
        if (n->right)
        {
            gen_jump(rgt_reg);
        }
        // Generate the false label
        gen_label(lft_reg);
        // Generate the right AST node (false clause) if there is an else clause
        if (n->right)
        {
            gen_ast(n->right, n, NO_LABEL);
            gen_free_regs();
            // Generate the end label
            gen_label(rgt_reg);
        }
        // Clean up the AST nodes
        free_AST_chld(n);
        return NO_REG;
    case A_LOOP:
        // Generate the loop start and end label
        lft_reg = label();
        rgt_reg = label();
        // Generate the code of start label
        gen_label(lft_reg);
        // Generate the left AST node (condition)
        gen_ast(n->left, n, rgt_reg);
        gen_free_regs();
        // Generate the right AST node (statement)
        gen_ast(n->right, n, NO_LABEL);
        gen_free_regs();
        // Generate the loop label jump
        gen_jump(lft_reg);
        // Generate the end label
        gen_label(rgt_reg);
        // Clean up the AST nodes
        free_AST_chld(n);
        return NO_REG;
    default:
        break;
    }

    // Generate the left AST node
    if (n->left)
    {
        lft_reg = gen_ast(n->left, n, NO_LABEL);
    }
    // Generate the right AST node
    if (n->right)
    {
        rgt_reg = gen_ast(n->right, n, NO_LABEL);
    }
    // Clean up the AST nodes
    free_AST_chld(n);

    // Handle the different AST node types
    switch (n->type)
    {
    case A_INTLIT:
        return gen_load_int(n->value.integer);
    case A_IDENT:
        return gen_load_global(n->value.integer);
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
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "ne", lbl);
        else
            return gen_cmp(lft_reg, rgt_reg, "e");
    case A_NE:
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "e", lbl);
        else
            return gen_cmp(lft_reg, rgt_reg, "ne");
    case A_LT:
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "ge", lbl);
        else
            return gen_cmp(lft_reg, rgt_reg, "l");
    case A_LE:
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "g", lbl);
        else
            return gen_cmp(lft_reg, rgt_reg, "le");
    case A_GT:
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "le", lbl);
        else
            return gen_cmp(lft_reg, rgt_reg, "g");
    case A_GE:
        if (prt && (prt->type == A_IFELSE || prt->type == A_LOOP))
            return gen_cmp_jump(lft_reg, rgt_reg, "l", lbl);
        else
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

// Generate the assembly code for the entire program
void gen_code()
{
    AST *seq;

    gen_free_regs();
    gen_load_lib();
    seq = sequence();
    gen_main_function_prologue();
    gen_ast(seq, NULL, NO_LABEL);
    gen_function_epilogue();
    free_AST(seq);
}
