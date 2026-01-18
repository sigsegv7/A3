//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module alu #(
    parameter ALU_SUB_IMM = 8'h01,
    parameter ALU_ADD_IMM = 8'h03
) (
    input  logic [7:0] operand_0,
    input  logic [7:0] operand_1,
    input logic  [7:0] opcode,
    output wire [7:0] result
);
    always_comb begin
        case (opcode)
            ALU_ADD_IMM: result = operand_0 + operand_1;
            ALU_SUB_IMM: result = operand_0 - operand_1;
            default: result = 0;
        endcase
    end
endmodule
