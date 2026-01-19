/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include "as3/ast.h"
#include "as3/ptrbox.h"

int
ast_alloc_node(struct as3_state *state, ast_type_t type, struct ast_node **res)
{
    struct ast_node *node;

    if (state == NULL || res == NULL) {
        errno = -EINVAL;
        return -1;
    }

    node = ptrbox_alloc(&state->ptrbox, sizeof(*node));
    if (node == NULL) {
        errno = -ENOMEM;
        return -1;
    }

    memset(node, 0, sizeof(*node));
    node->type = type;
    *res = node;
    return 0;
}
