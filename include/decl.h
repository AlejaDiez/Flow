/********************************************************************************
 * File Name: decl.h                                                            *
 * Description: This file contains all the prototypes for the functions used.   *
 ********************************************************************************/

// code/gen.c
int gen_code(AST *n);

// code/x86_64.c
void x86_64_free_regs();
void x86_64_free_reg(int reg);
int x86_64_alloc_reg();
void x86_64_load_lib();
void x86_64_function_prologue(const char *str);
void x86_64_function_epilogue();
int x86_64_load_int(int val);
int x86_64_add(int reg_1, int reg_2);
int x86_64_sub(int reg_1, int reg_2);
int x86_64_mul(int reg_1, int reg_2);
int x86_64_div(int reg_1, int reg_2);
int x86_64_mod(int reg_1, int reg_2);
void x86_64_print(int reg);

// parser/expr.c
AST *expression();

// parser/stmt.c
AST *statement();
AST *sequence();

// scanner/scan.c
void scan();
VALUE match(TOKEN_TYPE typ);

// utils/err.c
void fatal(const char *msg, int cod);
void syntax_error(const char *msg);
void unrecognized_char_error(char chr);
void unrecognized_token_error();
void match_error(const char *exptd, const char *fnd);
void compile_error(const char *msg);

// utils/tree.c
AST *make_AST_node(AST_TYPE typ, AST *lft, AST *rgt, VALUE val);
AST *make_AST_binary(AST_TYPE typ, AST *lft, AST *rgt, VALUE val);
AST *make_AST_unary(AST_TYPE typ, AST *chld, VALUE val);
AST *make_AST_leaf(AST_TYPE typ, VALUE val);
