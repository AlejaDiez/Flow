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

// Generate the assembly code for the main function
void x86_64_main_function_prologue()
{
    fprintf(
        Output,
        "\t.section __TEXT, __text\n"
        "\t.globl _main\n"
        "_main:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n");
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
void x86_64_global(int id)
{
    const SYMBOL sym = GlobalSymbols[id];

    if (sym.primitive == P_BOOL) // 1 byte
    {
        fprintf(
            Output,
            "\t.section __DATA, __bss\n"
            "\t.comm\t%s,1,1\n"
            "\n",
            sym.name);
    }
    else if (sym.primitive == P_INT) // 8 bytes
    {
        fprintf(
            Output,
            "\t.section __DATA, __bss\n"
            "\t.comm\t%s,8,8\n"
            "\n",
            sym.name);
    }
}

// Store a register value in a global variable and return the register number
int x86_64_store_global(int reg, int id)
{
    const SYMBOL sym = GlobalSymbols[id];

    if (sym.primitive == P_BOOL) // 1 byte
    {
        fprintf(Output, "\tmovb\t%s, %s(\%%rip)\n", bregs[reg], sym.name);
    }
    else if (sym.primitive == P_INT) // 8 bytes
    {
        fprintf(Output, "\tmovq\t%s, %s(\%%rip)\n", regs[reg], sym.name);
    }
    return reg;
}

// Load a global variable into a register and return the register number
int x86_64_load_global(int id)
{
    const SYMBOL sym = GlobalSymbols[id];
    int reg = x86_64_alloc_reg();

    if (sym.primitive == P_BOOL) // 1 byte
    {
        fprintf(Output, "\tmovzbq\t%s(\%%rip), %s\n", sym.name, regs[reg]);
    }
    else if (sym.primitive == P_INT) // 8 bytes
    {
        fprintf(Output, "\tmovq\t%s(\%%rip), %s\n", sym.name, regs[reg]);
    }
    return reg;
}

// Generate the assembly code for a function prologue
void x86_64_function_prologue(int id)
{
    const SYMBOL sym = GlobalSymbols[id];

    fprintf(
        Output,
        "\t.section __TEXT, __text\n"
        "\t.globl __%s\n"
        "__%s:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n",
        sym.name, sym.name);
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

// Check if one register is true and jump to a label if it is not
int x86_64_cond_jump(int reg, int lbl)
{
    fprintf(Output, "\tcmpq\t$1, %s\n", regs[reg]);
    fprintf(Output, "\tjne\tL%d\n", lbl);
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
