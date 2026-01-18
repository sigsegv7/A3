/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "as3/lexer.h"

/*
 * Check if a character is a whitespace and return
 * true if so.
 *
 * @state: Assembler state
 * @c:     Character to check
 */
static inline bool
lexer_is_ws(struct as3_state *state, char c)
{
    switch (c) {
    case '\n':
    case '\r':
    case '\t':
    case '\f':
    case ' ':
        return true;
    }

    return false;
}

/*
 * Consume a single character from the input file
 *
 * @lexer_consume: Assembler state
 * @skip_ws:       Skip whitespace
 *
 * Returns the consumed character on success, otherwise a
 * '\0' when there are no more bytes to consume.
 */
static char
lexer_consume(struct as3_state *state, bool skip_ws)
{
    char c;

    if (state == NULL) {
        return '\0';
    }

    /* Scan and skip whitespace */
    while (read(state->in_fd, &c, 1) > 0) {
        if (lexer_is_ws(state, c) && skip_ws) {
            continue;
        }

        return c;
    }

    return '\0';
}

int
lexer_scan(struct as3_state *state, struct token *res)
{
    char c;

    if (state == NULL || res == NULL) {
        errno = -EINVAL;
        return -1;
    }

    /* Consume a single byte */
    if ((c = lexer_consume(state, true)) == '\0') {
        return -1;
    }

    switch (c) {
    case ',':
        res->type = TT_COMMA;
        res->c = c;
        return 0;
    }

    return -1;
}
