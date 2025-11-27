#include <stdlib.h>
#include "ast.h"

Expr *ast_number(int value) {
    Expr *e = (Expr *)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->kind = EXPR_NUMBER;
    e->u.number.value = value;
    return e;
}

Expr *ast_unary(UnOp op, Expr *expr) {
    Expr *e = (Expr *)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->kind = EXPR_UNARY;
    e->u.unary.op = op;
    e->u.unary.expr = expr;
    return e;
}

Expr *ast_binary(BinOp op, Expr *left, Expr *right) {
    Expr *e = (Expr *)malloc(sizeof(Expr));
    if (!e) return NULL;
    e->kind = EXPR_BINARY;
    e->u.binary.op = op;
    e->u.binary.left = left;
    e->u.binary.right = right;
    return e;
}

void ast_free(Expr *expr) {
    if (!expr) return;
    switch (expr->kind) {
        case EXPR_NUMBER:
            break;
        case EXPR_UNARY:
            ast_free(expr->u.unary.expr);
            break;
        case EXPR_BINARY:
            ast_free(expr->u.binary.left);
            ast_free(expr->u.binary.right);
            break;
    }
    free(expr);
}
