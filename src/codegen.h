#ifndef CODEGEN_H
#define CODEGEN_H

#include <stddef.h>
#include "ast.h"

typedef enum {
    BC_PUSH_CONST = 0,
    BC_ADD,
    BC_SUB,
    BC_MUL,
    BC_DIV,
    BC_MOD,
    BC_NEG,
    BC_HALT
} OpCode;

typedef struct {
    int   *data;
    size_t count;
    size_t capacity;
} Bytecode;

void bc_init(Bytecode *bc);
void bc_emit(Bytecode *bc, int value);
void bc_free(Bytecode *bc);

void codegen_expr(Bytecode *bc, Expr *expr);
void codegen_finish(Bytecode *bc);

#endif /* CODEGEN_H */
