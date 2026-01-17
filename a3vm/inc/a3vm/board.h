/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef A3VM_BOARD_H
#define A3VM_BOARD_H 1

#include <stdint.h>
#include <stddef.h>
#include "a3vm/cpu.h"

#define MINIMUM_MEMORY 0x800000     /* 8 MiB */
#define MAXIMUM_MEMORY 0x80000000   /* 2 GiB */

/*
 * Represents the mainboard containing processors, memory
 * and things like that
 *
 * @cpus:    System processors
 * @ram:     Random access memory
 * @ram_sz:  Current RAM size
 * @ram_cap: Maximum RAM size
 */
struct mainboard {
    struct cpu_desc cpus[1];
    uint8_t *ram;
    size_t ram_sz;
    size_t ram_cap;
};

/*
 * Initialize the mainboard emulation
 *
 * @mbp: Mainboard descriptor pointer
 *
 * Returns zero on success
 */
int mainboard_init(struct mainboard *mbp);

/*
 * Write a single byte of memory to mainboard RAM
 *
 * @mbp:  Mainboard descriptor
 * @addr: Address to write to
 * @data: Data buffer to write
 * @count: Number of bytes to write
 */
int mainboard_ram_write(
    struct mainboard *mbp, uintptr_t addr,
    void *data, size_t count
);

/*
 * Allocate all resources associated with a mainboard
 * emulation context
 *
 * @mbp: Mainboard descriptor to deallocate
 */
void mainboard_power_off(struct mainboard *mbp);

#endif  /* !A3VM_BOARD_H */
