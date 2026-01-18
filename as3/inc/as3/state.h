/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_STATE_H
#define AS3_STATE_H 1

#include "as3/token.h"

/*
 * Represents the assembler state
 *
 * @in_fd:      Input source file fd
 * @last_token: Most previous token
 */
struct as3_state {
    int in_fd;
    struct token last_token;
};

/*
 * Initialize the assembler state
 *
 * @state: State to initialize
 * @path:  Path of input source file
 *
 * Returns zero on success
 */
int as3_state_init(struct as3_state *state, const char *path);

/*
 * Assembler state to destroy
 *
 * @state: Assembler state
 */
void as3_state_destroy(struct as3_state *state);

#endif  /* !AS3_STATE_H */
