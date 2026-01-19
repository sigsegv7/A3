/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include "as3/lexer.h"
#include "as3/parser.h"
#include "as3/token.h"
#include "as3/trace.h"
#include "as3/ast.h"
#include "as3/reg.h"
#include "as3/codegen.h"

/* Obtain most previous token */
#define curtok(state) \
    (&(state)->last_token)

/* Quoted token entry */
#define qtok(str) \
    "'" str "'"

/* Symbolic token entry */
#define symtok(str) \
    "<" str ">"

/* Convert token type to string */
#define tokstr1(tt)    \
    (toktab[(tt)])

/* Convert token to string */
#define tokstr(tok) \
    (tokstr1((tok)->type))

/* Unexpected EOF */
#define ueof(state)                 \
    trace_error(                    \
        (state),                    \
        "unexpected end of file\n"  \
    )

/* Unexpected token */
#define utok(state, exp, got)               \
    trace_error(                            \
        state,                              \
        "expected %s, got %s instead\n",    \
        (exp),                              \
        (got)                               \
    )

/* Unexpected token */
#define utok1(state, tok)                   \
    trace_error(                            \
        state,                              \
        "unexpected token %s\n",            \
        tokstr(tok)                         \
    )

/*
 * Token table used to convert numeric token types
 * into human readable values
 */
static const char *toktab[] = {
    [TT_NONE]       = symtok("none"),
    [TT_NEWLINE]    = symtok("newline"),
    [TT_COMMA]      = qtok(","),
    [TT_G0]         = qtok("g0"),
    [TT_G1]         = qtok("g1"),
    [TT_G2]         = qtok("g2"),
    [TT_G3]         = qtok("g3"),
    [TT_G4]         = qtok("g4"),
    [TT_G5]         = qtok("g5"),
    [TT_G6]         = qtok("g6"),
    [TT_G7]         = qtok("g7"),
    [TT_G8]         = qtok("g8"),
    [TT_G9]         = qtok("g9"),
    [TT_G10]        = qtok("g10"),
    [TT_G11]        = qtok("g11"),
    [TT_G12]        = qtok("g12"),
    [TT_G13]        = qtok("g13"),
    [TT_G14]        = qtok("g14"),
    [TT_G15]        = qtok("g15"),
    [TT_IDENT]      = symtok("ident"),
    [TT_MOV]        = qtok("mov"),
    [TT_ZERO]       = qtok("zero")
};

/*
 * Returns true if the given token any register
 */
static inline bool
parse_is_reg(struct token *tok)
{
    if (token_to_reg(tok->type) != REG_BAD) {
        return true;
    }

    return false;
}

/*
 * Calls into the lexer for a single token
 *
 * @state: Assembler state
 * @tok:   Last token
 *
 * Returns zero on success
 */
static inline int
parse_scan(struct as3_state *state, struct token *tok)
{
    if (state == NULL || tok == NULL) {
        return -1;
    }

    return lexer_scan(state, tok);
}

/*
 * Assert that the next token is of a specific
 * value
 *
 * @state: Assembler state
 * @tok:   Last token
 * @what:  What to expect
 */
static int
parse_expect(struct as3_state *state, struct token *tok, tt_t what)
{
    if (state == NULL || tok == NULL) {
        return -1;
    }

    if (parse_scan(state, tok) < 0) {
        ueof(state);
        return -1;
    }

    if (tok->type != what) {
        utok(state, tokstr1(what), tokstr(tok));
        return -1;
    }

    return 0;
}

/*
 * Begin parsing a 'mov'
 *
 * @state: Assembler state
 * @tok:   Last token
 * @res:   AST result
 *
 * Returns zero on success
 */
static int
parse_mov(struct as3_state *state, struct token *tok, struct ast_node **res)
{
    struct ast_node *left, *root, *right;
    reg_t rd, rs;

    if (state == NULL || tok == NULL) {
        return -1;
    }

    if (tok->type != TT_MOV) {
        return -1;
    }

    if (parse_scan(state, tok) < 0) {
        ueof(state);
        return -1;
    }

    /* EXPECT <register> */
    if (!parse_is_reg(tok)) {
        utok(state, symtok("register"), tokstr(tok));
        return -1;
    }

    if ((rd = token_to_reg(tok->type)) == REG_BAD) {
        utok(state, symtok("register"), tokstr(tok));
        return -1;
    }

    /* EXPECT ',' */
    if (parse_expect(state, tok, TT_COMMA) < 0) {
        return -1;
    }

    if (parse_scan(state, tok) < 0) {
        ueof(state);
        return -1;
    }

    /* EXPECT <register> */
    if (!parse_is_reg(tok)) {
        utok(state, symtok("register"), tokstr(tok));
        return -1;
    }

    if ((rs = token_to_reg(tok->type)) == REG_BAD) {
        utok(state, symtok("register"), tokstr(tok));
        return -1;
    }

    if (parse_expect(state, tok, TT_NEWLINE) < 0) {
        return -1;
    }

    if (ast_alloc_node(state, AST_MOV, &root) < 0) {
        trace_error(state, "failed to allocate AST_MOV\n");
        return -1;
    }

    if (ast_alloc_node(state, AST_REG, &left) < 0) {
        trace_error(state, "failed to allocate AST_REG\n");
        return -1;
    }

    if (ast_alloc_node(state, AST_REG, &right) < 0) {
        trace_error(state, "failed to allocate AST_REG\n");
        return -1;
    }

    left->reg = rd;
    right->reg = rs;

    root->left = left;
    root->right = right;
    *res = root;
    return 0;
}

/*
 * Begin parsing the input assembly file
 *
 * @state: Assembler state
 * @tok:   Last token
 *
 * Returns zero on success
 */
static int
parse_begin(struct as3_state *state, struct token *tok)
{
    struct ast_node *root = NULL;

    if (state == NULL || tok == NULL) {
        return -1;
    }

    switch (tok->type) {
    case TT_MOV:
        if (parse_mov(state, tok, &root) < 0) {
            return -1;
        }

        break;
    default:
        utok1(state, tok);
        return -1;
    }

    if (root != NULL) {
        return cg_assemble_node(state, root);
    }

    return 0;
}

int
parser_parse(struct as3_state *state)
{
    struct token *tok;

    if (state == NULL) {
        errno = -EINVAL;
        return -1;
    }

    while (parse_scan(state, &state->last_token) == 0) {
        tok = curtok(state);
        printf("got token %s\n", tokstr(tok));
        if (parse_begin(state, tok) < 0) {
            return -1;
        }
    }

    return 0;
}
