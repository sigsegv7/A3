//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module domain (
    input logic [5:0] regsel,

    input wire clk,
    input wire reset,
    output wire [7:0] alu_result
);
    alu alu (
        .operand_0(0),
        .operand_1(0),
        .opcode(0),
        .result(alu_result)
    );

    regbank rbk (
        .regsel(regsel), .reg_val(0),
        .reg_we(0), .reset(reset),
        .clk(clk)
    );
endmodule
