/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Valu.h"
#include "Valu___024root.h"

#define MAX_SIM_ITER 50

/* ALU opcodes */
#define ALU_ADD 0x00
#define ALU_SUB 0x01
#define ALU_OR  0x04
#define ALU_AND 0x05
#define ALU_XOR 0x06

int main(int argc, char** argv, char** env)
{
    Valu *alu = new Valu;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    alu->trace(m_trace, 5);
    m_trace->open("alu.vcd");

    for (int i = 0; i < MAX_SIM_ITER; ++i) {
        alu->operand_0 = i;
        alu->operand_1 = 1;

        switch (i) {
        case 0:
            alu->opcode = ALU_ADD;
            break;
        case 2:
            alu->opcode = ALU_SUB;
            break;
        case 4:
            alu->opcode = ALU_OR;
            break;
        case 6:
            alu->opcode = ALU_AND;
            break;
        case 8:
            alu->opcode = ALU_XOR;
            break;
        }

        alu->eval();
        m_trace->dump(i);
    }

    m_trace->close();
    delete alu;
    return 0;
}
