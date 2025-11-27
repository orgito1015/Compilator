#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer *lex;
} Parser;

void parser_init(Parser *p, Lexer *lex);
Expr *parse_expression(Parser *p);

#endif /* PARSER_H */
