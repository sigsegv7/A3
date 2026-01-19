//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module soc (
    input wire clk,
    input wire reset
);
    logic [7:0] bus_pool;

    domain cpu_domain0 (
        .clk(clk),
        .reset(reset),
        .bus_in(bus_pool)
    );

    busctl bus_unit (
        .write_en(0),
        .addr_in(0),
        .data_in(0),
        .clk(clk),
        .data_out(bus_pool)
    );
endmodule
