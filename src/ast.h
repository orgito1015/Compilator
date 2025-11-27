#ifndef AST_H
#define AST_H

typedef enum {
    EXPR_NUMBER,
    EXPR_UNARY,
    EXPR_BINARY
} ExprKind;

typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV
} BinOp;

typedef enum {
    UOP_NEG
} UnOp;

typedef struct Expr {
    ExprKind kind;
    union {
        struct {
            int value;
        } number;
        struct {
            UnOp op;
            struct Expr *expr;
        } unary;
        struct {
            BinOp op;
            struct Expr *left;
            struct Expr *right;
        } binary;
    } u;
} Expr;

Expr *ast_number(int value);
Expr *ast_unary(UnOp op, Expr *expr);
Expr *ast_binary(BinOp op, Expr *left, Expr *right);
void  ast_free(Expr *expr);

#endif /* AST_H */
