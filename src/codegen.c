#include <stdlib.h>
#include "codegen.h"

void bc_init(Bytecode *bc) {
    bc->data = NULL;
    bc->count = 0;
    bc->capacity = 0;
}

void bc_emit(Bytecode *bc, int value) {
    if (bc->count >= bc->capacity) {
        size_t new_cap = bc->capacity ? bc->capacity * 2 : 16;
        int *new_data = (int *)realloc(bc->data, new_cap * sizeof(int));
        if (!new_data) {
            /* Out of memory: abort */
            abort();
        }
        bc->data = new_data;
        bc->capacity = new_cap;
    }
    bc->data[bc->count++] = value;
}

void bc_free(Bytecode *bc) {
    free(bc->data);
    bc->data = NULL;
    bc->count = 0;
    bc->capacity = 0;
}

static void codegen_expr_internal(Bytecode *bc, Expr *expr) {
    switch (expr->kind) {
        case EXPR_NUMBER:
            bc_emit(bc, BC_PUSH_CONST);
            bc_emit(bc, expr->u.number.value);
            break;
        case EXPR_UNARY:
            codegen_expr_internal(bc, expr->u.unary.expr);
            if (expr->u.unary.op == UOP_NEG) {
                bc_emit(bc, BC_NEG);
            }
            break;
        case EXPR_BINARY:
            codegen_expr_internal(bc, expr->u.binary.left);
            codegen_expr_internal(bc, expr->u.binary.right);
            switch (expr->u.binary.op) {
                case OP_PLUS:
                    bc_emit(bc, BC_ADD);
                    break;
                case OP_MINUS:
                    bc_emit(bc, BC_SUB);
                    break;
                case OP_MUL:
                    bc_emit(bc, BC_MUL);
                    break;
                case OP_DIV:
                    bc_emit(bc, BC_DIV);
                    break;
            }
            break;
    }
}

void codegen_expr(Bytecode *bc, Expr *expr) {
    codegen_expr_internal(bc, expr);
}

void codegen_finish(Bytecode *bc) {
    bc_emit(bc, BC_HALT);
}
