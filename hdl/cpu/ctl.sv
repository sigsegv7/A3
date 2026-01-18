//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

//
// The control unit takes in control operations and
// manages the rest of the processor.
//
module ctl #(
    parameter CTL_NOP = 8'h00,
    parameter CTL_LOAD_IMM = 8'h01
) (
    input wire [7:0]  ctl_op,
    input wire [5:0]  reg_sel,
    input wire [63:0] imm,
    input wire clk,

    output logic regbank_we,
    output logic [5:0] regbank_sel,
    output logic [63:0] regbank_valout
);
    always @(posedge clk) begin
        case (ctl_op)
            CTL_NOP: ;
            CTL_LOAD_IMM: begin
                regbank_sel <= reg_sel;
                regbank_valout <= imm;
                regbank_we <= 1;
            end
            default: ;
        endcase
    end
endmodule
