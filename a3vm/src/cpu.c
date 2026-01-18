/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "a3vm/cpu.h"
#include "a3vm/board.h"

static int
cpu_fetch(struct cpu_desc *desc, struct mainboard *mbp, inst_t *res)
{
    struct cpu_regs *regs;
    int error;

    if (desc == NULL || mbp == NULL) {
        return -1;
    }

    if (res == NULL) {
        return -1;
    }

    regs = &desc->regs;
    error = mainboard_ram_read(
        mbp,
        regs->ip,
        res,
        4
    );

    if (error < 0) {
        perror("mainboard_ram_read");
        return -1;
    }

    return 0;
}

/*
 * Handle the immediate mov instruction
 */
static int
cpu_imov(struct cpu_desc *cpu, inst_t *inst)
{
    struct cpu_regs *regs;
    uint8_t rd;
    uint32_t imm;

    if (cpu == NULL || inst == NULL) {
        errno = -EINVAL;
        return -1;
    }

    if (inst_op(inst) != OPCODE_IMOV) {
        errno = -EINVAL;
        return -1;
    }

    regs = &cpu->regs;
    rd = (inst_raw(inst) >> 8) & 0xFF;
    imm = (inst_raw(inst) >> 16) & 0xFFFF;

    /* Is this a general purpose register? */
    if (rd >= REG_G0 && rd <= REG_G15) {
        regs->gpreg[rd - 1] = imm;
        return 0;
    }

    switch (rd) {
    case REG_SP:
        regs->sp = imm;
        break;
    case REG_FP:
        regs->fp = imm;
        break;
    default:
        printf("[!] decoded bad register %x\n", rd);
        return -1;
    }

    return 0;
}

int
cpu_power_up(struct cpu_desc *desc)
{
    struct cpu_regs *regs;

    if (desc == NULL) {
        errno = -EINVAL;
        return -1;
    }

    regs = &desc->regs;
    regs->ip = 0x00;
    regs->sp = 0x00;
    regs->fp = 0x00;
    memset(regs->gpreg, 0xFFFFFFFF, sizeof(regs->gpreg));
    return 0;
}

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
    size_t cycle = 0;

    if (desc == NULL) {
        errno = -EINVAL;
        return -1;
    }

    regs = &desc->regs;
    while (regs->ip < MAXIMUM_MEMORY) {
        if (cpu_fetch(desc, mbp, &inst) < 0) {
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
        case OPCODE_IMOV:
            if (cpu_imov(desc, &inst) < 0) {
                return -1;
            }

            regs->ip += 4;
            break;
        default:
            printf("[!] bad opcode %x\n", opcode);
            return -1;
        }

        printf("[*] cycle %zd complete\n", cycle++);
        cpu_dump(desc);
    }

    return 0;
}
