/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vdomain.h"
#include "Vdomain___024root.h"

#define MAX_SIM_ITER 50

int main(int argc, char** argv, char** env)
{
    Vdomain *domain = new Vdomain;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    domain->trace(m_trace, 5);
    m_trace->open("cpu.vcd");

    domain->clk = 0;
    for (int i = 0; i < MAX_SIM_ITER; ++i) {
        /* Reset on power-up */
        if (i == 0) {
            domain->reset = 1;
        }

        domain->eval();
        m_trace->dump(i);
        domain->clk ^= 1;

        /* De-assert reset after two clocks */
        if (i == 2) {
            domain->reset = 0;
        }
    }

    m_trace->close();
    delete domain;
    return 0;
}
