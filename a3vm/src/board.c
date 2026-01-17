/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "a3vm/board.h"
#include "a3vm/cpu.h"

static inline int
ram_try_balloon(struct mainboard *mbp, uintptr_t addr)
{
    void *tmp;

    if (mbp == NULL) {
        return -1;
    }

    /* Expand the RAM within the capacity if needed */
    if (addr >= mbp->ram_sz) {
        tmp = realloc(mbp->ram, addr);
        if (tmp == NULL)
            return -1;

        mbp->ram = tmp;
        mbp->ram_sz = addr;
    }

    return 0;
}

int
mainboard_init(struct mainboard *mbp)
{
    if (mbp == NULL) {
        errno = -EINVAL;
        return -1;
    }

    memset(mbp, 0, sizeof(*mbp));
    mbp->ram_cap = MAXIMUM_MEMORY;
    mbp->ram_sz = MINIMUM_MEMORY;
    if ((mbp->ram = malloc(mbp->ram_sz)) == NULL) {
        errno = -ENOMEM;
        return -1;
    }

    printf("[*] bsp up\n");
    cpu_dump(&mbp->cpus[0]);
    return 0;
}

int
mainboard_ram_write(struct mainboard *mbp, uintptr_t addr, void *data,
    size_t count)
{
    uint8_t *src;

    if (mbp == NULL || data == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if ((addr + count) >= MAXIMUM_MEMORY) {
        errno = -EIO;
        return -1;
    }

    if (ram_try_balloon(mbp, (addr + count)) < 0) {
        errno = -EIO;
        return -1;
    }

    src = (uint8_t *)data;
    for (size_t i = 0; i < count; ++i) {
        mbp->ram[addr + i] = src[i];
    }

    return 0;
}

void
mainboard_power_off(struct mainboard *mbp)
{
    free(mbp->ram);
}
