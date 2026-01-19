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

/* Obtain most previous token */
#define curtok(state) \
    (&(state)->last_token)

/* Quoted token entry */
#define qtok(str) \
    "'" str "'"

/* Symbolic token entry */
#define symtok(str) \
    "<" str ">"

/* Convert token to string */
#define tokstr(tok) \
    (toktab[(tok)->type])

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
    [TT_IDENT]      = symtok("ident")
};

int
parser_parse(struct as3_state *state)
{
    struct token *tok;

    if (state == NULL) {
        errno = -EINVAL;
        return -1;
    }

    while (lexer_scan(state, &state->last_token) == 0) {
        tok = curtok(state);
        printf("got token %s\n", tokstr(tok));
        if (tok->type == TT_IDENT) {
            printf("identifier: %s\n", tok->s);
        }
    }

    return 0;
}
