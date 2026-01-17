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

#define MAX_SIM_ITER 5

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
        alu->opcode = 0;

        alu->eval();

        m_trace->dump(i);
    }

    m_trace->close();
    delete alu;
    return 0;
}
