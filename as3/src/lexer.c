/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "as3/lexer.h"
#include "as3/ptrbox.h"

/*
 * Place a given character in the putback buffer
 *
 * @state: Assembler state
 * @c:     Character to put in putback buffer
 */
static inline void
lexer_putback(struct as3_state *state, char c)
{
    if (state == NULL) {
        return;
    }

    state->lex_putback = c;
}

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

    /*
     * If there is something in the putback buffer for us to
     * read, take it.
     */
    if ((c = state->lex_putback) != '\0') {
        state->lex_putback = '\0';
        if (lexer_is_ws(state, c) && !skip_ws)
            return c;
        if (!lexer_is_ws(state, c))
            return c;
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

/*
 * Scan for an identifier token
 *
 * @state: Assembler state
 * @lc:    Last character
 * @res:   Token result
 *
 * Returns zero on success
 */
static int
lexer_scan_ident(struct as3_state *state, int lc, struct token *res)
{
    char *buf;
    size_t bufcap, bufsz;
    char c;

    if (state == NULL || res == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if (!isalpha(lc) && lc != '_') {
        errno = -EINVAL;
        return -1;
    }

    bufcap = 8;
    bufsz = 0;
    if ((buf = malloc(bufcap)) == NULL) {
        errno = -ENOMEM;
        return -1;
    }

    buf[bufsz++] = lc;
    for (;;) {
        if ((c = lexer_consume(state, true)) == '\0') {
            buf[bufsz] = '\0';
            break;
        }

        if (!isalnum(c) && c != '_') {
            lexer_putback(state, c);
            buf[bufsz] = '\0';
            break;
        }

        buf[bufsz++] = c;
        if (bufsz >= bufcap - 1) {
            bufcap += 8;
            buf = realloc(buf, bufcap);
        }
    }

    res->type = TT_IDENT;
    res->s = ptrbox_strdup(&state->ptrbox, buf);
    free(buf);
    return 0;
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
    case '\n':
        res->type = TT_NEWLINE;
        res->c = c;
        return 0;
    case ',':
        res->type = TT_COMMA;
        res->c = c;
        return 0;
    default:
        if (lexer_scan_ident(state, c, res) == 0) {
            return 0;
        }

        break;
    }

    return -1;
}
