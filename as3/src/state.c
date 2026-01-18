/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include "as3/state.h"

int
as3_state_init(struct as3_state *state, const char *path)
{
    if (state == NULL || path == NULL) {
        errno = -EINVAL;
        return -1;
    }

    state->in_fd = open(path, O_RDONLY);
    if (state->in_fd < 0) {
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
}
