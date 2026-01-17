/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef A3VM_CPU_H
#define A3VM_CPU_H 1

#include <stdint.h>
#include <stddef.h>

#define N_GPREG 16

/*
 * Represents processor registers
 *
 * @gpreg: General purpose registers
 * @ip:    Instruction pointer
 * @sp:    Stack pointer
 * @fp:    Frame pointer
 */
struct cpu_regs {
    uint64_t gpreg[N_GPREG];
    uint64_t ip;
    uint64_t sp;
    uint64_t fp;
};

/*
 * Represents a single processor core
 *
 * @id:   ID of processor core
 * @regs: Processor registers
 */
struct cpu_desc {
    uint8_t id;
    struct cpu_regs regs;
};

/*
 * Dump information relating to a processor descriptor
 *
 * @desc: Processor descriptor to dump
 */
void cpu_dump(struct cpu_desc *desc);

#endif  /* !A3VM_CPU_H */
