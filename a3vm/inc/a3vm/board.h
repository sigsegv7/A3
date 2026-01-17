/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef A3VM_BOARD_H
#define A3VM_BOARD_H 1

#include <stdint.h>
#include <stddef.h>
#include "a3vm/cpu.h"

/*
 * Represents the mainboard containing processors, memory
 * and things like that
 *
 * @cpus: System processors
 */
struct mainboard {
    struct cpu_desc cpus[1];
};

/*
 * Initialize the mainboard emulation
 *
 * @mbp: Mainboard descriptor pointer
 *
 * Returns zero on success
 */
int mainboard_init(struct mainboard *mbp);

#endif  /* !A3VM_BOARD_H */
