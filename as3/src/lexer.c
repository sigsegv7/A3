/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
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
        if ((c = lexer_consume(state, false)) == '\0') {
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

/*
 * Scan for a number
 *
 * @state: Assembler state
 * @lc:    Last character
 * @res:   Token result is written here
 *
 * Returns zero on success
 */
static int
lexer_scan_number(struct as3_state *state, int lc, struct token *res)
{
    char buf[25];
    uint8_t buf_i;
    uint8_t base = 10;
    char c;

    if (state == NULL || res == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if (!isdigit(lc)) {
        errno = -EINVAL;
        return -1;
    }

    buf_i = 0;
    if (lc != '0') {
        buf[buf_i++] = lc;
    }

    /* Handle base */
    if (lc == '0') {
        if ((c = lexer_consume(state, true)) == '\0')
            return -1;
    }

    /* Base-16? */
    if (c == 'x') {
        if ((c = lexer_consume(state, true)) == '\0')
            return -1;

        buf[buf_i++] = c;
        base = 16;
    }

    for (;;) {
        c = lexer_consume(state, false);
        if (!isdigit(c) && !isxdigit(c)) {
            buf[buf_i] = '\0';
            lexer_putback(state, c);
            break;
        }

        buf[buf_i++] = c;
        if (buf_i >= sizeof(buf) - 1) {
            return -1;
        }
    }

    res->type = TT_NUMBER;
    res->v = strtol(buf, NULL, base);
    return 0;
}

/*
 * Check an indentifier token against a list of general purpose
 * registers and override the type if it matches.
 *
 * @tok:   Token to check
 *
 * Returns zero on success
 */
static int
lexer_gpreg(struct token *tok)
{
    uint8_t reg_num;
    char *p;
    tt_t reg_lookup[] = {
        TT_G0,   TT_G1,
        TT_G2,   TT_G3,
        TT_G4,   TT_G5,
        TT_G6,   TT_G7,
        TT_G8,   TT_G9,
        TT_G10,  TT_G11,
        TT_G12,  TT_G13,
        TT_G14,  TT_G15
    };

    if (tok == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if (tok->type != TT_IDENT) {
        errno = -EINVAL;
        return -1;
    }

    /* Is this actual a general purpose register? */
    p = tok->s;
    if (*(p++) != 'g') {
        errno = -EINVAL;
        return -1;
    }

    /*
     * atoi() returns zero on error so we need to check if we
     * actually have an error or just register g0.
     */
    reg_num = atoi(p);
    if (reg_num == 0) {
        if (strcmp(tok->s, "g0") == 0) {
            tok->type = TT_G0;
            return 0;
        }

        errno = -EINVAL;
        return -1;
    }

    /* There are only 16 gpregs */
    if (reg_num > 15) {
        errno = -EINVAL;
        return -1;
    }

    tok->type = reg_lookup[reg_num];
    return 0;
}

/*
 * Check an indentifier token against a list of known keywords
 * and override the type if it matches.
 *
 * @tok:   Token to check
 *
 * Returns zero on success
 */
static int
lexer_kw(struct token *tok)
{
    if (tok == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if (tok->type != TT_IDENT) {
        errno = -EINVAL;
        return -1;
    }

    switch (*tok->s) {
    case 'g':
        if (lexer_gpreg(tok) == 0) {
            return 0;
        }

        break;
    case 'm':
        if (strcmp(tok->s, "mov") == 0) {
            tok->type = TT_MOV;
            return 0;
        }

        break;
    case 'z':
        if (strcmp(tok->s, "zero") == 0) {
            tok->type = TT_ZERO;
            return 0;
        }

        break;
    case 'h':
        if (strcmp(tok->s, "hlt") == 0) {
            tok->type = TT_HLT;
            return 0;
        }

        break;
    case 's':
        if (strcmp(tok->s, "srr") == 0) {
            tok->type = TT_SRR;
            return 0;
        }

        break;
    }

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
            lexer_kw(res);
            return 0;
        }

        if (lexer_scan_number(state, c, res) == 0) {
            return 0;
        }

        break;
    }

    return -1;
}
