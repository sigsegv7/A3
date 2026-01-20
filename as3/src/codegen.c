/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include "as3/codegen.h"
#include "as3/trace.h"

#define cg_emitb(state, byte) do {          \
        uint8_t b = (byte);                 \
                                            \
        write((state)->out_fd, &b, 1);      \
    } while (0);

/* Valid opcodes */
#define OP_HLT      0x06        /* HLT */
#define OP_MOV_REG  0x0E        /* MOV REG  [A] */

/*
 * A lookup table used to convert register types
 * into valid values that can be encoded into
 * te instruction.
 */
uint8_t regtab[] = {
    [REG_G0] = 0x00,
    [REG_G1] = 0x01,
    [REG_G2] = 0x02,
    [REG_G3] = 0x03,
    [REG_G4] = 0x04,
    [REG_G5] = 0x05,
    [REG_G6] = 0x06,
    [REG_G7] = 0x07,
    [REG_G8] = 0x08,
    [REG_G9] = 0x09,
    [REG_G10] = 0x0A,
    [REG_G11] = 0x0B,
    [REG_G12] = 0x0C,
    [REG_G13] = 0x0D,
    [REG_G14] = 0x0E,
    [REG_G15] = 0x0F,
    [REG_ZERO] = 0x12
};

/*
 * Convert a register identifier into a value that
 * can be encoded.
 *
 * @reg: Register ID to convert
 *
 * Returns REG_BAD on failure
 */
static inline uint8_t
cg_reg_op(reg_t reg)
{
    if (reg >= REG_MAX) {
        return REG_BAD;
    }

    return regtab[reg];
}

static int
cg_emit_mov(struct as3_state *state, struct ast_node *root)
{
    struct ast_node *left, *right;
    uint8_t rd, rs;

    if (state == NULL || root == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if ((left = root->left) == NULL) {
        trace_error(state, "MOV has no lhs\n");
        return -1;
    }

    if ((right = root->right) == NULL) {
        trace_error(state, "MOV has no lhs\n");
        return -1;
    }

    if (left->type != AST_REG) {
        trace_error(state, "lhs is not AST_REG\n");
        return -1;
    }

    if (right->type != AST_REG) {
        trace_error(state, "rhs is not AST_REG\n");
        return -1;
    }

    if ((rd = cg_reg_op(left->reg)) == REG_BAD) {
        trace_error(state, "bad lhs register\n");
        return -1;
    }

    if ((rs = cg_reg_op(right->reg)) == REG_BAD) {
        trace_error(state, "bad rhs register\n");
        return -1;
    }

    cg_emitb(state, OP_MOV_REG);
    cg_emitb(state, rd);
    cg_emitb(state, rs);
    cg_emitb(state, 0x00);
    return 0;
}

static int
cg_emit_hlt(struct as3_state *state, struct ast_node *root)
{
    if (state == NULL || root == NULL) {
        errno = -EINVAL;
        return -1;
    }

    cg_emitb(state, OP_HLT);
    return 0;
}

int
cg_assemble_node(struct as3_state *state, struct ast_node *root)
{
    if (root == NULL) {
        errno = -EINVAL;
        return -1;
    }

    switch (root->type) {
    case AST_MOV:
        if (cg_emit_mov(state, root) < 0) {
            return -1;
        }

        return 0;
    case AST_HLT:
        if (cg_emit_hlt(state, root) < 0) {
            return -1;
        }

        return 0;
    default:
        trace_warn("got bad node type %d\n", 1);
        break;
    }

    return -1;
}
