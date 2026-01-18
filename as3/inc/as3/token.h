/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_TOKEN_H
#define AS3_TOKEN_H 1

#include <stdint.h>
#include <stddef.h>

/*
 * Represents valid token types
 */
typedef enum {
    TT_COMMA,   /* ','   */
    TT_G0,      /* 'g0'  */
    TT_G1,      /* 'g1'  */
    TT_G2,      /* 'g2'  */
    TT_G3,      /* 'g3'  */
    TT_G4,      /* 'g4'  */
    TT_G5,      /* 'g5'  */
    TT_G6,      /* 'g6'  */
    TT_G7,      /* 'g7'  */
    TT_G8,      /* 'g8'  */
    TT_G9,      /* 'g9'  */
    TT_G10,     /* 'g10' */
    TT_G11,     /* 'g11' */
    TT_G12,     /* 'g12' */
    TT_G13,     /* 'g13' */
    TT_G14,     /* 'g14' */
    TT_G15,     /* 'g15' */
    TT_IDENT,   /* <IDENT> */
} tt_t;

/*
 * Represents a lexical token
 *
 * @type: Token type
 */
struct token {
    tt_t type;
    union {
        char c;
        char *s;
    };
};

#endif  /* !AS3_TOKEN_H */
