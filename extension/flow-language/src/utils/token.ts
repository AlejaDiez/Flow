export const T_EOF = 0;
// Arithmetic Operators
export const T_PLUS = 1;
export const T_MINUS = 2;
export const T_STAR = 3;
export const T_SLASH = 4;
export const T_PERCENT = 5;
// Relational Operators
export const T_EQUAL = 6;
export const T_NOT_EQUAL = 7;
export const T_LESS = 8;
export const T_LESS_EQUAL = 9;
export const T_GREATER = 10;
export const T_GREATER_EQUAL = 11;
// Assignment Operators
export const T_ASSIGN = 12;
// Literals and Identifiers
export const T_INTLIT = 13;
export const T_TRUE = 14;
export const T_FALSE = 15;
export const T_IDENT = 16;
// Types
export const T_VOID = 17;
export const T_BOOL = 18;
export const T_INT = 19;
// Keywords
export const T_VAR = 20;
export const T_FUN = 21;
export const T_IF = 22;
export const T_ELSE = 23;
export const T_LOOP = 24;
export const T_PRINT = 25;
// Punctuation
export const T_COLON = 26;
export const T_SEMICOLON = 27;
// Gruping
export const T_LPAREN = 28;
export const T_RPAREN = 29;
export const T_LBRACE = 30;
export const T_RBRACE = 31;

export interface Token {
    type: number;
    value: string;
}
