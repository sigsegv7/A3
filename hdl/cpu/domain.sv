//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module domain (
    input wire clk,
    input wire reset,
    input logic [7:0] bus_in,

    output logic bus_we,
    output logic [16:0] bus_addr
);
    // Register bank blocks
    logic regbank_we;
    logic [5:0] regbank_sel;
    logic [63:0] regbank_val;

    // Bus unit interface blocks
    logic ctl_bus_we;
    logic [16:0] ctl_bus_addr;

    /* verilator lint_off UNUSEDSIGNAL */
    logic [7:0] alu_result;
    logic [7:0] ctl_data_pool;

    alu alu (
        .operand_0(0),
        .operand_1(0),
        .opcode(0),
        .result(alu_result)
    );

    regbank rbk (
        .regsel(regbank_sel), .reg_val(regbank_val),
        .reg_we(regbank_we), .reset(reset),
        .clk(clk)
    );

    ctl ctl_unit (
        .ctl_op(0), .bus_in(bus_in),
        .reg_sel(1), .data_in(0),
        .clk(clk),
        .bus_data_in(bus_in),
        .regbank_we(regbank_we),
        .regbank_sel(regbank_sel),
        .regbank_valout(regbank_val),
        .bus_we(ctl_bus_we),
        .addr_out(ctl_bus_addr),
        .data_out(ctl_data_pool)
    );

    // Latch the outputs per clock rising edge
    always @(posedge clk) begin
        bus_we <= ctl_bus_we;
        bus_addr <= ctl_bus_addr;
    end
endmodule
