//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module soc (
    input wire clk,
    input wire reset
);
    /* verilator lint_off UNUSEDSIGNAL */
    logic [7:0] mem_read_pool;

    memctl mem_ctl (
        .data_in(0),  .addr_in(0),
        .write_en(0), .read_en(0),
        .clk(clk),
        .data_out(mem_read_pool)
    );

    domain cpu_domain0 (
        .clk(clk),
        .reset(reset)
    );
endmodule
