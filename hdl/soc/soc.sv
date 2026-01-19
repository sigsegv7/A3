//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module soc (
    input wire clk,
    input wire reset
);
    /* verilator lint_off UNUSEDSIGNAL */
    logic [7:0] data_pool;

    domain cpu_domain0 (
        .clk(clk),
        .reset(reset)
    );

    busctl bus_unit (
        .write_en(0),
        .addr_in(0),
        .data_in(0),
        .clk(clk),
        .data_out(data_pool)
    );
endmodule
