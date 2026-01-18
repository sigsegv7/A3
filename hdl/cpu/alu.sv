//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module alu #(
    parameter ALU_NOP = 8'h00,
    parameter ALU_ADD = 8'h01,
    parameter ALU_SUB = 8'h02,
    parameter ALU_OR  = 8'h03,
    parameter ALU_AND = 8'h04,
    parameter ALU_XOR = 8'h05
) (
    input  logic [7:0] operand_0,
    input  logic [7:0] operand_1,
    input logic  [7:0] opcode,
    output wire [7:0] result
);
    always_comb begin
        case (opcode)
            ALU_NOP: result = 0;
            ALU_ADD: result = operand_0 + operand_1;
            ALU_SUB: result = operand_0 - operand_1;
            ALU_OR:  result = operand_0 | operand_1;
            ALU_AND: result = operand_0 & operand_1;
            ALU_XOR: result = operand_0 ^ operand_1;
            default: result = 0;
        endcase
    end
endmodule
