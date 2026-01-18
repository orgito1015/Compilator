#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"
#include "ast.h"

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(f);
        return NULL;
    }
    long size = ftell(f);
    if (size < 0) {
        perror("ftell");
        fclose(f);
        return NULL;
    }
    rewind(f);

    char *buffer = (char *)malloc((size_t)size + 1);
    if (!buffer) {
        perror("malloc");
        fclose(f);
        return NULL;
    }

    size_t read_bytes = fread(buffer, 1, (size_t)size, f);
    fclose(f);

    buffer[read_bytes] = '\0';
    return buffer;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "OrionCC — Minimal C Subset Compiler\n");
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *source = read_file(argv[1]);
    if (!source) {
        return EXIT_FAILURE;
    }

    Lexer lex;
    lexer_init(&lex, source);

    Parser parser;
    parser_init(&parser, &lex);

    Expr *expr = parse_expression(&parser);

    if (lex.current_token.kind != TOK_EOF) {
        fprintf(stderr, "Error: extra tokens after valid expression\n");
        ast_free(expr);
        free(source);
        return EXIT_FAILURE;
    }

    Bytecode bc;
    bc_init(&bc);
    codegen_expr(&bc, expr);
    codegen_finish(&bc);

    int result = vm_execute(&bc);
    printf("Result: %d\n", result);

    ast_free(expr);
    bc_free(&bc);
    free(source);

    return EXIT_SUCCESS;
}
