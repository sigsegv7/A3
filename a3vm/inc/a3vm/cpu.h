/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef A3VM_CPU_H
#define A3VM_CPU_H 1

#include <stdint.h>
#include <stddef.h>

/*
 * Represents a single processor core
 *
 * @id: ID of processor core
 */
struct cpu_desc {
    uint8_t id;
};

#endif  /* !A3VM_CPU_H */
