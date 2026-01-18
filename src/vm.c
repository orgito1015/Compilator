#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

#define STACK_MAX 1024

int vm_execute(const Bytecode *bc) {
    int stack[STACK_MAX];
    int sp = 0; /* stack pointer */
    size_t ip = 0; /* instruction pointer */

    while (ip < bc->count) {
        int op = bc->data[ip++];

        switch (op) {
            case BC_PUSH_CONST: {
                if (ip >= bc->count) {
                    fprintf(stderr, "Bytecode error: missing constant after BC_PUSH_CONST\n");
                    exit(EXIT_FAILURE);
                }
                int value = bc->data[ip++];
                if (sp >= STACK_MAX) {
                    fprintf(stderr, "VM error: stack overflow\n");
                    exit(EXIT_FAILURE);
                }
                stack[sp++] = value;
                break;
            }
            case BC_ADD: {
                if (sp < 2) { fprintf(stderr, "VM error: stack underflow on ADD\n"); exit(EXIT_FAILURE); }
                int b = stack[--sp];
                int a = stack[--sp];
                stack[sp++] = a + b;
                break;
            }
            case BC_SUB: {
                if (sp < 2) { fprintf(stderr, "VM error: stack underflow on SUB\n"); exit(EXIT_FAILURE); }
                int b = stack[--sp];
                int a = stack[--sp];
                stack[sp++] = a - b;
                break;
            }
            case BC_MUL: {
                if (sp < 2) { fprintf(stderr, "VM error: stack underflow on MUL\n"); exit(EXIT_FAILURE); }
                int b = stack[--sp];
                int a = stack[--sp];
                stack[sp++] = a * b;
                break;
            }
            case BC_DIV: {
                if (sp < 2) { fprintf(stderr, "VM error: stack underflow on DIV\n"); exit(EXIT_FAILURE); }
                int b = stack[--sp];
                int a = stack[--sp];
                if (b == 0) {
                    fprintf(stderr, "VM error: division by zero\n");
                    exit(EXIT_FAILURE);
                }
                stack[sp++] = a / b;
                break;
            }
            case BC_MOD: {
                if (sp < 2) { fprintf(stderr, "VM error: stack underflow on MOD\n"); exit(EXIT_FAILURE); }
                int b = stack[--sp];
                int a = stack[--sp];
                if (b == 0) {
                    fprintf(stderr, "VM error: modulo by zero\n");
                    exit(EXIT_FAILURE);
                }
                stack[sp++] = a % b;
                break;
            }
            case BC_NEG: {
                if (sp < 1) { fprintf(stderr, "VM error: stack underflow on NEG\n"); exit(EXIT_FAILURE); }
                int a = stack[--sp];
                stack[sp++] = -a;
                break;
            }
            case BC_HALT: {
                if (sp < 1) {
                    fprintf(stderr, "VM error: no result on stack at HALT\n");
                    exit(EXIT_FAILURE);
                }
                return stack[sp - 1];
            }
            default:
                fprintf(stderr, "VM error: unknown opcode %d\n", op);
                exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "VM error: no BC_HALT encountered\n");
    exit(EXIT_FAILURE);
}
