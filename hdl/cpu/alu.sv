//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module alu #(
    parameter ALU_ADD = 4'b0000,
    parameter ALU_SUB = 4'b0001
) (
    input  logic [7:0] operand_0,
    input  logic [7:0] operand_1,
    input logic  [3:0] opcode,
    output wire [7:0] result
);
    always_comb begin
        case (opcode)
            ALU_ADD: result = operand_0 + operand_1;
            ALU_SUB: result = operand_0 - operand_1;
            default: result = 0;
        endcase
    end
endmodule
