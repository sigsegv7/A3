//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module soc (
    input wire clk,
    input wire reset,

    input wire [2:0] utap_sel,
    input wire utap_rx,

    output wire utap_out
);
    logic pc_inhibit;
    logic [7:0] bus_pool;
    logic [16:0] bus_addr;
    logic bus_we;

    domain cpu_domain0 (
        .pc_inhibit(pc_inhibit),
        .clk(clk),
        .reset(reset),
        .bus_in(bus_pool),
        .bus_we(bus_we),
        .bus_addr(bus_addr)
    );

    dbgctl dbg_unit (
        .clk(clk),
        .sel(utap_sel),
        .dbg_rx(utap_rx),
        .dbg_tx(utap_out),
        .pc_inhibit_out(pc_inhibit)
    );

    busctl bus_unit (
        .write_en(bus_we),
        .addr_in(bus_addr),
        .data_in(0),
        .clk(clk),
        .data_out(bus_pool)
    );
endmodule
