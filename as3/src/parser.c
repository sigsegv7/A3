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
 * Returns true if the given token is a general
 * purpose register
 */
static inline bool
parse_is_gpreg(struct token *tok)
{
    switch (tok->type) {
    case TT_G0:
    case TT_G1:
    case TT_G2:
    case TT_G3:
    case TT_G4:
    case TT_G5:
    case TT_G6:
    case TT_G7:
    case TT_G8:
    case TT_G9:
    case TT_G10:
    case TT_G11:
    case TT_G12:
    case TT_G13:
    case TT_G14:
    case TT_G15:
    case TT_ZERO:
        return true;
    default:
        return false;
    }

    return false;
}

/*
 * Returns true if the given token any register
 */
static inline bool
parse_is_reg(struct token *tok)
{
    if (parse_is_gpreg(tok)) {
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
 *
 * Returns zero on success
 */
static int
parse_mov(struct as3_state *state, struct token *tok)
{
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

    if (parse_expect(state, tok, TT_NEWLINE) < 0) {
        return -1;
    }

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
    if (state == NULL || tok == NULL) {
        return -1;
    }

    switch (tok->type) {
    case TT_MOV:
        if (parse_mov(state, tok) < 0) {
            return -1;
        }

        return 0;
    default:
        utok1(state, tok);
        return -1;
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
