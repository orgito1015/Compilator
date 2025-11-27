#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
    TOK_EOF = 0,
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN
} TokenKind;

typedef struct {
    TokenKind kind;
    int       int_value; /* valid if kind == TOK_NUMBER */
} Token;

typedef struct {
    const char *input;
    size_t      pos;
    char        current;
    Token       current_token;
} Lexer;

void lexer_init(Lexer *lex, const char *input);
void lexer_next(Lexer *lex);

#endif /* LEXER_H */
