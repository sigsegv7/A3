/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef PARSER_H
#define PARSER_H 1

#include "as3/state.h"

/*
 * Begin parsing the input source file
 *
 * @state: Assembler state
 *
 * Returns zero on success
 */
int parser_parse(struct as3_state *state);

#endif  /* !PARSER_H */
