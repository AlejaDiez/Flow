import { TextDocument } from 'vscode';
import {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_PERCENT,
    T_EQUAL,
    T_NOT_EQUAL,
    T_LESS,
    T_LESS_EQUAL,
    T_GREATER,
    T_GREATER_EQUAL,
    T_ASSIGN,
    T_INTLIT,
    T_TRUE,
    T_FALSE,
    T_IDENT,
    T_VOID,
    T_BOOL,
    T_INT,
    T_VAR,
    T_FUN,
    T_IF,
    T_ELSE,
    T_LOOP,
    T_PRINT,
    T_COLON,
    T_SEMICOLON,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    Token,
} from './token';

export default class Lexer {
    private text: string;
    private i: number;

    constructor(document: TextDocument) {
        this.text = document.getText();
        this.i = 0;
    }

    private back(): void {
        this.i--;
    }

    private next(): string {
        return this.text[this.i++];
    }

    private skip(): string {
        let c = this.next();

        while (
            c === ' ' ||
            c === '\t' ||
            c === '\r' ||
            c === '\f' ||
            c === '\n'
        ) {
            c = this.next();
        }
        return c;
    }

    nextToken(): Token {
        let c = this.skip();

        switch (c) {
            case undefined:
                return { type: T_EOF, value: '\0' };
            case '+':
                return { type: T_PLUS, value: '+' };
            case '-':
                return { type: T_MINUS, value: '-' };
            case '*':
                return { type: T_STAR, value: '*' };
            case '/':
                return { type: T_SLASH, value: '/' };
            case '%':
                return { type: T_PERCENT, value: '%' };
            case '=':
                c = this.next();
                if (c === '=') {
                    return { type: T_EQUAL, value: '==' };
                } else {
                    this.back();
                    return { type: T_ASSIGN, value: '=' };
                }
            case '!':
                c = this.next();
                if (c === '=') {
                    return { type: T_NOT_EQUAL, value: '!=' };
                } else {
                    throw new Error('Unexpected character: ' + c);
                }
            case '<':
                c = this.next();
                if (c === '=') {
                    return { type: T_LESS_EQUAL, value: '<=' };
                } else {
                    this.back();
                    return { type: T_LESS, value: '<' };
                }
            case '>':
                c = this.next();
                if (c === '=') {
                    return { type: T_GREATER_EQUAL, value: '>=' };
                } else {
                    this.back();
                    return { type: T_GREATER, value: '>' };
                }
            case ':':
                return { type: T_COLON, value: ':' };
            case ';':
                return { type: T_SEMICOLON, value: ';' };
            case '(':
                return { type: T_LPAREN, value: '(' };
            case ')':
                return { type: T_RPAREN, value: ')' };
            case '{':
                return { type: T_LBRACE, value: '{' };
            case '}':
                return { type: T_RBRACE, value: '}' };
            default:
                if (/[0-9]/.test(c)) {
                    let value = c;

                    c = this.next();
                    while (/[0-9]/.test(c)) {
                        value += c;
                        c = this.next();
                    }
                    this.back();
                    return { type: T_INTLIT, value: value };
                }
                if (/[_a-zA-Z]/.test(c)) {
                    let value = c;

                    c = this.next();
                    while (/[_a-zA-Z0-9]/.test(c)) {
                        value += c;
                        c = this.next();
                    }
                    this.back();
                    switch (value) {
                        case 'bool':
                            return { type: T_BOOL, value: 'bool' };
                        case 'else':
                            return { type: T_ELSE, value: 'else' };
                        case 'false':
                            return { type: T_FALSE, value: 'false' };
                        case 'fun':
                            return { type: T_FUN, value: 'fun' };
                        case 'if':
                            return { type: T_IF, value: 'if' };
                        case 'int':
                            return { type: T_INT, value: 'int' };
                        case 'loop':
                            return { type: T_LOOP, value: 'loop' };
                        case 'print':
                            return { type: T_PRINT, value: 'print' };
                        case 'true':
                            return { type: T_TRUE, value: 'true' };
                        case 'var':
                            return { type: T_VAR, value: 'var' };
                        case 'void':
                            return { type: T_VOID, value: 'void' };
                    }
                    return { type: T_IDENT, value: value };
                }
                throw new Error('Unexpected character: ' + c);
        }
    }

    *tokens(): IterableIterator<Token> {
        let token;

        while ((token = this.nextToken()).type !== T_EOF) {
            yield token;
        }
    }
}
