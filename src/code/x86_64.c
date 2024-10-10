/********************************************************************************
 * File Name: x86_64.c                                                          *
 * Description: This file contains all the specific x86_64 code generation.     *
 ********************************************************************************/

#include "defs.h"
#include "data.h"
#include "decl.h"

// Register list
static char *regs[4] = {"%r8", "%r9", "%r10", "%r11"};
static char *bregs[4] = {"%r8b", "%r9b", "%r10b", "%r11b"};
// Register availability
static bool free_regs[4];

// Set all registers as available
void x86_64_free_regs()
{
    free_regs[0] = free_regs[1] = free_regs[2] = free_regs[3] = true;
}

// Free a register by setting it as available
void x86_64_free_reg(int reg)
{
    char buff[128];

    if (free_regs[reg])
    {
        snprintf(buff, sizeof(buff), "error trying to free register '%d'", reg);
        compile_error(buff);
    }
    free_regs[reg] = true;
}

// Allocate a free register, return the number of the register.
int x86_64_alloc_reg()
{
    for (int i = 0; i < 4; i++)
    {
        if (free_regs[i])
        {
            free_regs[i] = false;
            return i;
        }
    }
    compile_error("out of registers");
    return -1;
}

// Load standard librearies functions
void x86_64_load_lib()
{
    // Print function
    fprintf(
        Output,
        "\t.section __DATA, __data\n"
        ".LC0:\n"
        "\t.string\t\"%%d\\n\"\n"
        "\n");
    fprintf(
        Output,
        "\t.section __TEXT, __text\n"
        "_print:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n"
        "\tsubq\t$16, %%rsp\n"
        "\tmovl\t%%edi, -4(%%rbp)\n"
        "\tmovl\t-4(%%rbp), %%eax\n"
        "\tmovl\t%%eax, %%esi\n"
        "\tleaq	.LC0(%%rip), %%rdi\n"
        "\tmovl	$0, %%eax\n"
        "\tcall	_printf\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n");
}

// Generate a label
void x86_64_label(int lbl)
{
    fprintf(Output, "L%d:\n", lbl);
}

// Generate a jump to a label
void x86_64_jump(int lbl)
{
    fprintf(Output, "\tjmp\tL%d\n", lbl);
}

// Generate the assembly code for a global variable
void x86_64_global(const char *str)
{
    fprintf(
        Output,
        "\t.section __DATA, __bss\n"
        "\t.comm\t%s,8,8\n"
        "\n",
        str);
}

// Store a register value in a global variable and return the register number
int x86_64_store_global(int reg, const char *str)
{
    fprintf(Output, "\tmovq\t%s, %s(\%%rip)\n", regs[reg], str);
    return reg;
}

// Load a global variable into a register and return the register number
int x86_64_load_global(const char *str)
{
    int reg = x86_64_alloc_reg();

    fprintf(Output, "\tmovq\t%s(\%%rip), %s\n", str, regs[reg]);
    return reg;
}

// Generate the assembly code for a function prologue
void x86_64_function_prologue(const char *str)
{
    const char *prefix = strcmp(str, "main") == 0 ? "_" : "__";

    fprintf(
        Output,
        "\t.section __TEXT, __text\n"
        "\t.globl %s%s\n"
        "%s%s:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n",
        prefix, str, prefix, str);
}

// Generate the assembly code for a function epilogue
void x86_64_function_epilogue()
{
    fprintf(
        Output,
        "\tmovl\t$0, %%eax\n"
        "\tpopq\t%%rbp\n"
        "\tret\n"
        "\n");
}

// Load an integer literal value into a register and return the register number
int x86_64_load_int(int val)
{
    int reg = x86_64_alloc_reg();

    fprintf(Output, "\tmovq\t$%d, %s\n", val, regs[reg]);
    return reg;
}

// Generate the assembly code for an additon operation and return the register number
int x86_64_add(int reg_1, int reg_2)
{
    fprintf(Output, "\taddq\t%s, %s\n", regs[reg_2], regs[reg_1]);
    x86_64_free_reg(reg_2);
    return reg_1;
}

// Generate the assembly code for a subtraction operation and return the register number
int x86_64_sub(int reg_1, int reg_2)
{
    fprintf(Output, "\tsubq\t%s, %s\n", regs[reg_2], regs[reg_1]);
    x86_64_free_reg(reg_2);
    return reg_1;
}

// Generate the assembly code for a multiplication operation and return the register number
int x86_64_mul(int reg_1, int reg_2)
{
    fprintf(Output, "\timulq\t%s, %s\n", regs[reg_2], regs[reg_1]);
    x86_64_free_reg(reg_2);
    return reg_1;
}

// Generate the assembly code for a division operation and return the register number
int x86_64_div(int reg_1, int reg_2)
{
    fprintf(
        Output,
        "\tmovq\t%s,%%rax\n"
        "\tcqo\n"
        "\tidivq\t%s\n"
        "\tmovq\t%%rax,%s\n",
        regs[reg_1], regs[reg_2], regs[reg_1]);
    x86_64_free_reg(reg_2);
    return reg_1;
}

// Generate the assembly code for a modulus operation and return the register number
int x86_64_mod(int reg_1, int reg_2)
{
    fprintf(
        Output,
        "\tmovq\t%s,%%rax\n"
        "\tcqo\n"
        "\tidivq\t%s\n"
        "\tmovq\t%%rdx,%s\n",
        regs[reg_1], regs[reg_2], regs[reg_1]);
    x86_64_free_reg(reg_2);
    return reg_1;
}

// Generate the assembly code for a comparison operation and return the register number
int x86_64_cmp(int reg_1, int reg_2, const char *cmp)
{
    fprintf(
        Output,
        "\tcmpq\t%s, %s\n"
        "\tset%s\t%s\n"
        "\tmovzbq\t%s, %s\n",
        regs[reg_2], regs[reg_1], cmp, bregs[reg_2], bregs[reg_2], regs[reg_2]);
    x86_64_free_reg(reg_1);
    return reg_2;
}

// Compare two registers and store the result in the first register and return the register number
int x86_64_cmp_jump(int reg_1, int reg_2, const char *jmp, int lbl)
{
    fprintf(Output, "\tcmpq\t%s, %s\n", regs[reg_2], regs[reg_1]);
    fprintf(Output, "\tj%s\tL%d\n", jmp, lbl);
    x86_64_free_regs();
    return NO_REG;
}

// Generate the assembly code for a print statement
void x86_64_print(int reg)
{
    fprintf(
        Output,
        "\tmovq\t%s, %%rdi\n"
        "\tcall\t_print\n",
        regs[reg]);
    x86_64_free_reg(reg);
}
