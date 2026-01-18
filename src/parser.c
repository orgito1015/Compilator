#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static Expr *parse_expr(Parser *p);
static Expr *parse_term(Parser *p);
static Expr *parse_factor(Parser *p);

static void parser_error(const char *msg) {
    fprintf(stderr, "Parse error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void parser_init(Parser *p, Lexer *lex) {
    p->lex = lex;
}

/* Grammar:
 * expr   := term (('+' | '-') term)*
 * term   := factor (('*' | '/') factor)*
 * factor := NUMBER
 *        | '-' factor
 *        | '(' expr ')'
 */

Expr *parse_expression(Parser *p) {
    return parse_expr(p);
}

static Expr *parse_expr(Parser *p) {
    Expr *left = parse_term(p);

    while (p->lex->current_token.kind == TOK_PLUS ||
           p->lex->current_token.kind == TOK_MINUS) {
        TokenKind op = p->lex->current_token.kind;
        lexer_next(p->lex);
        Expr *right = parse_term(p);
        if (op == TOK_PLUS) {
            left = ast_binary(OP_PLUS, left, right);
        } else {
            left = ast_binary(OP_MINUS, left, right);
        }
    }

    return left;
}

static Expr *parse_term(Parser *p) {
    Expr *left = parse_factor(p);

    while (p->lex->current_token.kind == TOK_STAR ||
           p->lex->current_token.kind == TOK_SLASH ||
           p->lex->current_token.kind == TOK_PERCENT) {
        TokenKind op = p->lex->current_token.kind;
        lexer_next(p->lex);
        Expr *right = parse_factor(p);
        if (op == TOK_STAR) {
            left = ast_binary(OP_MUL, left, right);
        } else if (op == TOK_SLASH) {
            left = ast_binary(OP_DIV, left, right);
        } else {
            left = ast_binary(OP_MOD, left, right);
        }
    }

    return left;
}

static Expr *parse_factor(Parser *p) {
    Token tok = p->lex->current_token;

    if (tok.kind == TOK_NUMBER) {
        lexer_next(p->lex);
        return ast_number(tok.int_value);
    }

    if (tok.kind == TOK_MINUS) {
        lexer_next(p->lex);
        Expr *inner = parse_factor(p);
        return ast_unary(UOP_NEG, inner);
    }

    if (tok.kind == TOK_LPAREN) {
        lexer_next(p->lex); /* consume '(' */
        Expr *e = parse_expr(p);
        if (p->lex->current_token.kind != TOK_RPAREN) {
            parser_error("expected ')'");
        }
        lexer_next(p->lex); /* consume ')' */
        return e;
    }

    parser_error("unexpected token in factor");
    return NULL;
}
