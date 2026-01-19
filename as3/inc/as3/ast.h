/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_AST_H
#define AS3_AST_H 1

#include <stdint.h>
#include <stddef.h>
#include "as3/state.h"
#include "as3/reg.h"

/*
 * Represents valid AST node types
 */
typedef enum {
    AST_MOV,
    AST_REG,
} ast_type_t;

/*
 * Represents an abstract syntax tree node
 *
 * @type: AST node type
 * @left: Left node
 * @right: Right node
 */
struct ast_node {
    ast_type_t type;
    struct ast_node *left;
    struct ast_node *right;
    union {
        reg_t reg;
    };
};

/*
 * Allocate an abstract syntax tree node
 *
 * @state: Assembler state
 * @type:  Type of node to create
 * @res:   Result is written here
 *
 * Returns zero on success
 */
int ast_alloc_node(struct as3_state *state, ast_type_t type, struct ast_node **res);

#endif  /* !AS3_AST_H */
