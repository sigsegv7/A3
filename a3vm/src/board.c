/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include "a3vm/board.h"

int
mainboard_init(struct mainboard *mbp)
{
    if (mbp == NULL) {
        errno = -EINVAL;
        return -1;
    }

    memset(mbp, 0, sizeof(*mbp));
    return 0;
}
