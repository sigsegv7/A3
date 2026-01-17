/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <errno.h>
#include "a3vm/cpu.h"
#include "a3vm/board.h"

void
cpu_dump(struct cpu_desc *desc)
{
    struct cpu_regs *regs;

    if (desc == NULL) {
        return;
    }

    printf("cpu=%d\n", desc->id);

    /* Write general purpose registers */
    regs = &desc->regs;
    for (uint8_t i = 0; i < N_GPREG; ++i) {
        if ((i % 2) == 0) {
            if (i > 0) printf("\n");
        }
        printf("G%d=0x%016zX ", i, regs->gpreg[i]);
    }

    /* Stack related registers and IP */
    printf("\nIP=0x%016zX\n", regs->ip);
    printf("SP=0x%016zX\n", regs->sp);
    printf("FP=0x%016zX\n", regs->fp);
}

int
cpu_run(struct cpu_desc *desc, struct mainboard *mbp)
{
    struct cpu_regs *regs;
    uint8_t opcode;
    inst_t inst;
    int error;
    size_t cycle = 0;

    if (desc == NULL) {
        errno = -EINVAL;
        return -1;
    }

    regs = &desc->regs;
    while (regs->ip < MAXIMUM_MEMORY) {
        error = mainboard_ram_read(
            mbp,
            regs->ip,
            &inst,
            4
        );

        if (error < 0) {
            perror("mainboard_ram_read");
            return -1;
        }

        opcode = inst_op(&inst);
        switch (opcode) {
        case OPCODE_NOP:
            ++regs->ip;
            break;
        case OPCODE_HLT:
            printf("[*] processor %d halted\n", desc->id);
            ++regs->ip;
            return 0;
        default:
            printf("[!] bad opcode %x\n", opcode);
            return -1;
        }

        printf("[*] cycle %zd complete\n", cycle++);
        cpu_dump(desc);
    }

    return 0;
}
