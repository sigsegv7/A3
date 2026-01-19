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
 * Load a register using its number
 *
 * @regs: Registers
 * @rd: Destination register
 * @v:  Value to load
 */
static int
cpu_reg_store(struct cpu_regs *regs, uint8_t rd, uint64_t v)
{
    if (rd >= REG_MAX) {
        return -1;
    }

    /* Fake the write, cannot write to ZERO reg */
    if (rd == REG_ZERO) {
        return 0;
    }

    /* Write the correct register */
    if (rd >= REG_G0 && rd <= REG_G15) {
        regs->gpreg[rd] = v;
    } else if (rd == REG_SP) {
        regs->sp = v;
    } else if (rd == REG_FP) {
        regs->fp = v;
    } else {
        return -1;
    }

    return 0;
}

/*
 * Load a value from a register
 *
 * @regs: Registers
 * @rs: Source register
 * @res: Result is written here
 *
 * Returns zero on success
 */
static int
cpu_reg_load(struct cpu_regs *regs, uint8_t rs, uint64_t *res)
{
    uint64_t tmp;

    if (res == NULL || rs >= REG_MAX) {
        return -1;
    }

    /* Read the correct register */
    if (rs >= REG_G0 && rs <= REG_G15) {
        tmp = regs->gpreg[rs];
    } else if (rs == REG_SP) {
        tmp = regs->sp;
    } else if (rs == REG_FP) {
        tmp = regs->fp;
    } else if (rs == REG_ZERO) {
        tmp = 0;
    } else {
        return -1;
    }

    *res = tmp;
    return 0;
}

/*
 * Perform a btype IMM operation
 */
static int
cpu_btype_iop(struct cpu_desc *cpu, inst_t *inst)
{
    struct cpu_regs *regs;
    uint8_t opcode, rd;
    uint32_t imm;
    uint64_t tmp;

    if (cpu == NULL || inst == NULL) {
        errno = -EINVAL;
        return -1;
    }

    regs = &cpu->regs;
    opcode = inst_op(inst);
    rd = (inst_raw(inst) >> 8) & 0xFF;
    imm = (inst_raw(inst) >> 16) & 0xFFFF;

    if (cpu_reg_load(regs, rd, &tmp) < 0) {
        printf("[!] failed to load register in iop\n");
        return -1;
    }

    switch (opcode) {
    case OPCODE_IOR:
        tmp |= imm;
        break;
    case OPCODE_IAND:
        tmp &= imm;
        break;
    case OPCODE_IXOR:
        tmp ^= imm;
        break;
    case OPCODE_IMOV:
        tmp = imm;
        break;
    case OPCODE_IADD:
        tmp += imm;
        break;
    }

    return cpu_reg_store(regs, rd, tmp);
}

static int
cpu_atype_op(struct cpu_desc *desc, inst_t *inst)
{
    struct cpu_regs *regs;
    uint8_t opcode;
    uint8_t rd0, rs1;
    uint64_t tmp_src;

    if (desc == NULL || inst == NULL) {
        errno = -EINVAL;
        return -1;
    }

    regs = &desc->regs;
    opcode = inst_op(inst);
    rd0 = (inst_raw(inst) >> 8) & 0xFF;
    rs1 = (inst_raw(inst) >> 16) & 0xFF;

    switch (opcode) {
    case OPCODE_MOV:
        if (cpu_reg_load(regs, rs1, &tmp_src) < 0) {
            printf("[!] failed to load register in op\n");
            return -1;
        }

        return cpu_reg_store(regs, rd0, tmp_src);
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
        case OPCODE_IOR:
        case OPCODE_IXOR:
        case OPCODE_IAND:
        case OPCODE_IMOV:
        case OPCODE_IADD:
            if (cpu_btype_iop(desc, &inst) < 0) {
                return -1;
            }

            regs->ip += 4;
            break;
        case OPCODE_MOV:
            if (cpu_atype_op(desc, &inst) < 0) {
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
