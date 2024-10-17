import {
    DocumentFormattingEditProvider,
    OutputChannel,
    TextDocument,
    FormattingOptions,
    CancellationToken,
    TextEdit,
    Range,
} from 'vscode';
import Lexer from './utils/lexer';
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
} from './utils/token';

export default class FlowFormatter implements DocumentFormattingEditProvider {
    private console: OutputChannel;

    constructor(console: OutputChannel) {
        this.console = console;
    }

    provideDocumentFormattingEdits(
        document: TextDocument,
        { insertSpaces, tabSize }: FormattingOptions,
        token: CancellationToken
    ): TextEdit[] {
        const tokens = [...new Lexer(document).tokens()];
        let stack: number[] = [];
        let formattedCode = '';
        let indentLevel = 0;
        let inLine = false;

        const exists = (token: number) => {
            return !!tokens[token];
        };

        const match = (token: number, types: number[]) => {
            if (exists(token)) {
                return types.includes(tokens[token].type);
            }
            return false;
        };

        const indent = () =>
            (insertSpaces ? ' ' : '\t').repeat(tabSize * indentLevel);

        tokens.forEach(({ type, value }, i) => {
            switch (type) {
                // Operators
                case T_PLUS:
                case T_MINUS:
                case T_STAR:
                case T_SLASH:
                case T_PERCENT:
                case T_EQUAL:
                case T_NOT_EQUAL:
                case T_LESS:
                case T_LESS_EQUAL:
                case T_GREATER:
                case T_GREATER_EQUAL:
                case T_ASSIGN:
                    formattedCode += ` ${value} `;
                    break;
                // Keywords
                case T_VAR:
                    formattedCode += 'var ';
                    stack.push(T_VAR);
                    break;
                case T_FUN:
                    formattedCode += 'fun ';
                    stack.push(T_VAR);
                    break;
                case T_IF:
                    formattedCode += 'if ';
                    break;
                case T_ELSE:
                    formattedCode += 'else';
                    // Check for space
                    if (match(i + 1, [T_IF])) {
                        formattedCode += ' ';
                    }
                    break;
                case T_LOOP:
                    formattedCode += 'loop ';
                    inLine = true;
                    break;
                // Punctuation
                case T_COLON:
                    formattedCode += ': ';
                    break;
                case T_SEMICOLON:
                    formattedCode += ';';
                    if (!inLine) {
                        formattedCode += '\n';
                        if (
                            exists(i + 1) &&
                            stack[stack.length - 1] === T_VAR
                        ) {
                            stack.pop();
                            if (
                                !match(i + 1, [T_VAR]) &&
                                !match(i + 1, [T_RBRACE])
                            ) {
                                formattedCode += '\n';
                            }
                        } else if (match(i + 1, [T_VAR, T_FUN])) {
                            formattedCode += '\n';
                        }
                        if (!match(i + 1, [T_RBRACE])) {
                            formattedCode += indent();
                        }
                    } else {
                        formattedCode += ' ';
                    }
                    break;
                // Grouping
                case T_LBRACE:
                    stack.push(T_LBRACE);
                    indentLevel++;
                    formattedCode += ' {';
                    if (!match(i + 1, [T_RBRACE])) {
                        formattedCode += '\n';
                        // Add indentation
                        formattedCode += indent();
                    }
                    break;
                case T_RBRACE:
                    stack.pop();
                    indentLevel--;
                    formattedCode += indent();
                    formattedCode += '}';
                    // Check for new line
                    if (match(i + 1, [T_ELSE])) {
                        formattedCode += ' ';
                    } else {
                        formattedCode += '\n';
                        if (
                            exists(i + 1) &&
                            stack[stack.length - 1] === T_FUN
                        ) {
                            stack.pop();
                            formattedCode += '\n';
                        } else if (match(i + 1, [T_VAR, T_FUN])) {
                            formattedCode += '\n';
                        }
                        if (!match(i + 1, [T_RBRACE])) {
                            formattedCode += indent();
                        }
                    }
                    break;
                case T_LPAREN:
                    formattedCode += '(';
                    break;
                case T_RPAREN:
                    formattedCode += ')';
                    inLine = false;
                    break;
                // Literals and Identifiers
                // Types
                default:
                    formattedCode += value;
                    break;
            }
        });
        return [
            TextEdit.replace(
                new Range(
                    document.positionAt(0),
                    document.positionAt(document.getText().length)
                ),
                formattedCode
            ),
        ];
    }
}
