#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

static void lexer_advance(Lexer *lex) {
    if (lex->input[lex->pos] == '\0') {
        lex->current = '\0';
    } else {
        lex->current = lex->input[lex->pos++];
    }
}

static void lexer_skip_whitespace(Lexer *lex) {
    while (isspace((unsigned char)lex->current)) {
        lexer_advance(lex);
    }
}

void lexer_init(Lexer *lex, const char *input) {
    lex->input = input;
    lex->pos = 0;
    lexer_advance(lex);
    lexer_next(lex);
}

void lexer_next(Lexer *lex) {
    lexer_skip_whitespace(lex);

    if (lex->current == '\0') {
        lex->current_token.kind = TOK_EOF;
        return;
    }

    if (isdigit((unsigned char)lex->current)) {
        int value = 0;
        while (isdigit((unsigned char)lex->current)) {
            int digit = lex->current - '0';
            /* Check for overflow before multiplication */
            if (value > (INT_MAX - digit) / 10) {
                fprintf(stderr, "Lexer error: number too large (overflow)\n");
                exit(EXIT_FAILURE);
            }
            value = value * 10 + digit;
            lexer_advance(lex);
        }
        lex->current_token.kind = TOK_NUMBER;
        lex->current_token.int_value = value;
        return;
    }

    switch (lex->current) {
        case '+':
            lex->current_token.kind = TOK_PLUS;
            lexer_advance(lex);
            return;
        case '-':
            lex->current_token.kind = TOK_MINUS;
            lexer_advance(lex);
            return;
        case '*':
            lex->current_token.kind = TOK_STAR;
            lexer_advance(lex);
            return;
        case '/':
            lex->current_token.kind = TOK_SLASH;
            lexer_advance(lex);
            return;
        case '%':
            lex->current_token.kind = TOK_PERCENT;
            lexer_advance(lex);
            return;
        case '(':
            lex->current_token.kind = TOK_LPAREN;
            lexer_advance(lex);
            return;
        case ')':
            lex->current_token.kind = TOK_RPAREN;
            lexer_advance(lex);
            return;
        default:
            /* Unknown character: treat as EOF for now. */
            lex->current_token.kind = TOK_EOF;
            lexer_advance(lex);
            return;
    }
}
