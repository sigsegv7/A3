/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "as3/state.h"

int
as3_state_init(struct as3_state *state, const char *path)
{
    if (state == NULL || path == NULL) {
        errno = -EINVAL;
        return -1;
    }

    memset(state, 0, sizeof(*state));
    state->in_fd = open(path, O_RDONLY);
    if (state->in_fd < 0) {
        return -1;
    }

    state->out_fd = open(OUTPUT_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (state->out_fd < 0) {
        close(state->in_fd);
        return -1;
    }

    if (ptrbox_init(&state->ptrbox) < 0) {
        close(state->in_fd);
        return -1;
    }

    return 0;
}

void
as3_state_destroy(struct as3_state *state)
{
    if (state == NULL) {
        return;
    }

    close(state->in_fd);
    close(state->out_fd);
    ptrbox_destroy(&state->ptrbox);
}
