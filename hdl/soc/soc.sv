//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module soc (
    input wire clk,
    input wire reset
);
    logic [7:0] bus_pool;
    logic [16:0] bus_addr;
    logic bus_we;

    domain cpu_domain0 (
        .pc_inhibit(0),
        .clk(clk),
        .reset(reset),
        .bus_in(bus_pool),
        .bus_we(bus_we),
        .bus_addr(bus_addr)
    );

    busctl bus_unit (
        .write_en(bus_we),
        .addr_in(bus_addr),
        .data_in(0),
        .clk(clk),
        .data_out(bus_pool)
    );
endmodule
