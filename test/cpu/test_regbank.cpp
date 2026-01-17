/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vregbank.h"
#include "Vregbank___024root.h"

#define MAX_SIM_ITER 50

int main(int argc, char** argv, char** env)
{
    Vregbank *rbk = new Vregbank;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    rbk->trace(m_trace, 5);
    m_trace->open("regbank.vcd");

    rbk->clk = 0;
    for (int i = 0; i < MAX_SIM_ITER; ++i) {
        /* Reset on power-up */
        if (i == 0) {
            rbk->reset = 1;
        }

        /* Set some registers on the 4th clock */
        if (i == 4) {
            rbk->g0_in = 0xDEADBEEF;
            rbk->g1_in = 0x0CACACA7;
        }

        rbk->eval();
        m_trace->dump(i);
        rbk->clk ^= 1;

        /* De-assert reset after two clocks */
        if (i == 2) {
            rbk->reset = 0;
        }
    }

    m_trace->close();
    delete rbk;
    return 0;
}
