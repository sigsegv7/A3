/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_LEXER_H
#define AS3_LEXER_H 1

#include "as3/token.h"
#include "as3/state.h"

/*
 * Scan for a single token
 *
 * @state: Assembler state
 * @res:   Token result is written here
 *
 * Returns zero on success
 */
int lexer_scan(struct as3_state *state, struct token *res);

#endif  /* !AS3_LEXER_H */
