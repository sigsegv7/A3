//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module domain (
    input wire clk,
    input wire reset
);
    logic regbank_we;
    logic [5:0] regbank_sel;
    logic [63:0] regbank_val;
    /* verilator lint_off UNUSEDSIGNAL */
    logic [7:0] alu_result;

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
        .ctl_op(0), .reg_sel(1),
        .imm(0), .clk(clk),
        .regbank_we(regbank_we),
        .regbank_sel(regbank_sel),
        .regbank_valout(regbank_val)
    );
endmodule
