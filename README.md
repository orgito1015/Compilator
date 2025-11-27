# OrionCC — Minimal C Subset Compiler

**OrionCC** is a small, didactic **C subset compiler** and **stack‑based virtual machine**, implemented in C.

It is designed as a *professional, extensible* starting point for experimenting with compiler construction:

- Lexing (tokenization)
- Parsing (recursive‑descent, precedence‑aware)
- AST construction and ownership
- Bytecode generation
- Execution on a simple, well‑defined VM

> ⚠️ OrionCC does **not** implement the full C language.  
> In its current version, it focuses on **pure integer expressions** with C‑like syntax, in order to keep the core compiler pipeline clear and easy to extend.

---

## 1. Supported language subset

OrionCC currently accepts a single expression per input file, with the following features:

- **Integer literals** (decimal):
  - `0`, `1`, `42`, `1337`, ...
- **Binary operators** with standard precedence and associativity:
  - `+` (addition)
  - `-` (subtraction)
  - `*` (multiplication)
  - `/` (integer division, truncating toward zero)
- **Unary minus**:
  - `-x`
- **Parentheses** to override precedence:
  - `(1 + 2) * 3`

Examples of valid input:

```c
1 + 2 * (3 + 4)
-5 + 10 * (2 + 3)
(1 + 2 + 3) / 2
```

---

## 2. Architecture overview

The project is intentionally split into clear, focused modules:

- `src/lexer.*`  
  Hand‑written lexer that converts the input character stream into tokens (`NUMBER`, `+`, `-`, `*`, `/`, `(`, `)`, `EOF`).

- `src/parser.*`  
  Recursive‑descent parser implementing the following grammar:

  ```text
  expr   := term   (('+' | '-') term)*
  term   := factor (('*' | '/') factor)*
  factor := NUMBER
          | '-' factor
          | '(' expr ')'
  ```

  The parser builds a typed **AST** (see `ast.h`).

- `src/ast.*`  
  Definition and management of expression nodes:
  - `EXPR_NUMBER`
  - `EXPR_UNARY`
  - `EXPR_BINARY`

  Includes constructors and a recursive `ast_free` to avoid memory leaks.

- `src/codegen.*`  
  Translates the AST into a simple, linear **bytecode** format, using opcodes such as:
  - `BC_PUSH_CONST`
  - `BC_ADD`, `BC_SUB`, `BC_MUL`, `BC_DIV`
  - `BC_NEG`
  - `BC_HALT`

- `src/vm.*`  
  A compact stack‑based VM that executes the bytecode and returns the final result as an `int`.  
  Includes runtime checks for stack underflow, division by zero, and missing `BC_HALT`.

- `src/main.c`  
  Command‑line driver:
  - reads the input file
  - runs the compiler pipeline (lex → parse → codegen → execute)
  - prints: `Result: <value>`

This structure mirrors the layout of real compilers on a smaller scale, making it suitable for teaching, experimentation, or as a base for building a richer C subset.

---

## 3. Build instructions

### Requirements

- A C compiler (e.g. `gcc`, `clang`)
- `make`

### Steps

```bash
cd orioncc
make
```

This will produce:

- `bin/orioncc`

---

## 4. Usage

Compile and run one of the examples:

```bash
./bin/orioncc examples/example1.oc
```

Example output:

```text
Result: 17
```

where `examples/example1.oc` contains:

```c
1 + 2 * (3 + 4)
```

You can also provide your own input file:

```bash
echo "10 - 3 * 2" > myexpr.oc
./bin/orioncc myexpr.oc
# Result: 4
```

---

## 5. Extending OrionCC

OrionCC is intentionally minimal but structured for growth. Potential next steps:

1. **Richer expression language**
   - Comparison operators: `<`, `>`, `<=`, `>=`, `==`, `!=`
   - Logical operators: `&&`, `||`
   - Modulo operator: `%`

2. **Statements and a tiny IR**
   - Variable declarations and assignments:
     ```c
     int x = 3;
     x = x + 1;
     ```
   - A simple statement grammar:
     ```text
     program  := stmt_list
     stmt_list:= stmt*
     stmt     := expr ';'
              |  "print" '(' expr ')' ';'
     ```

3. **Control flow**
   - `if` / `else`, `while`, `for` mapped to jumps in bytecode
   - Basic block representation if you want to experiment with optimizations

4. **Towards real C**
   - `int main(void) { ... }` as an entry point
   - Function calls and a call stack in the VM
   - Eventually, emitting native assembly instead of bytecode

OrionCC is not intended to compete with GCC/Clang; instead, it aims to be a **clear, professional‑quality reference implementation** for a compact compiler pipeline.

---

## 6. License

You can treat this as an educational starting point and adapt it for your own projects or teaching material.  
If you publish it, consider adding a standard open‑source license (e.g. MIT) and crediting the project name **OrionCC**.
