/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_CODEGEN_H
#define AS3_CODEGEN_H

#include "as3/ast.h"
#include "as3/state.h"

/*
 * Begin generating machine code for the give root
 * node.
 *
 * @state: Assembler state
 * @root: Root node to assemble from
 *
 * Returns zero on success
 */
int cg_assemble_node(struct as3_state *state, struct ast_node *root);

#endif  /* AS3_CODEGEN_H */
