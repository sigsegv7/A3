//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module busctl (
    input wire write_en,
    input wire [16:0] addr_in,
    input wire [7:0]  data_in,
    input wire clk,

    output [7:0] data_out
);
    memctl mem_ctl (
        .data_in(data_in),
        .addr_in(addr_in),
        .write_en(write_en),
        .clk(clk),
        .data_out(data_out)
    );
endmodule
