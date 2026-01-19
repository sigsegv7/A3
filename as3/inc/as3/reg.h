/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_REG_H
#define AS3_REG_H 1

#include "as3/token.h"

typedef enum {
    REG_G0, REG_G1,
    REG_G2, REG_G3,
    REG_G4, REG_G5,
    REG_G6, REG_G7,
    REG_G8, REG_G9,
    REG_G10, REG_G11,
    REG_G12, REG_G13,
    REG_G14, REG_G15,
    REG_ZERO,
    REG_MAX,
    REG_BAD = 0xFF
} reg_t;

static inline reg_t
token_to_reg(tt_t token)
{
    switch (token) {
    case TT_G0: return REG_G0;
    case TT_G1: return REG_G1;
    case TT_G2: return REG_G2;
    case TT_G3: return REG_G3;
    case TT_G4: return REG_G4;
    case TT_G5: return REG_G5;
    case TT_G6: return REG_G6;
    case TT_G7: return REG_G7;
    case TT_G8: return REG_G8;
    case TT_G9: return REG_G9;
    case TT_G10: return REG_G10;
    case TT_G11: return REG_G11;
    case TT_G12: return REG_G12;
    case TT_G13: return REG_G13;
    case TT_G14: return REG_G14;
    case TT_G15: return REG_G15;
    case TT_ZERO: return REG_ZERO;
    default:
        return REG_BAD;

    }

    return REG_BAD;
}

#endif  /* !AS3_REG_H */
