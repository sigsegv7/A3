/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef A3VM_CPU_H
#define A3VM_CPU_H 1

#include <stdint.h>
#include <stddef.h>

/* Runtime parameters */
#define N_GPREG 16

/* Valid opcodes */
#define OPCODE_NOP      0x00    /* No-operation */
#define OPCODE_HLT      0x06    /* Halt */

/* Forward declaration */
struct mainboard;

/*
 * Represents a single instruction
 */
typedef union {
    struct {
        uint8_t opcode;
        uint8_t operand[3];
    };
    uint32_t raw;
} inst_t;

/* Instruction access helpers */
#define inst_op(inst)  ((inst)->opcode)
#define inst_raw(inst) ((inst)->raw)

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
 * Initialize a processor descriptor
 *
 * @desc: Descriptor to initialize
 *
 * Returns zero on success
 */
int cpu_power_up(struct cpu_desc *desc);

/*
 * Begin processor execution
 *
 * @desc: Processor descriptor
 * @mbp:  Mainboard descriptor
 */
int cpu_run(struct cpu_desc *desc, struct mainboard *mbp);

/*
 * Dump information relating to a processor descriptor
 *
 * @desc: Processor descriptor to dump
 */
void cpu_dump(struct cpu_desc *desc);

#endif  /* !A3VM_CPU_H */
