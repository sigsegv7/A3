//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module regbank #(
    parameter GP_RESET_VALUE = 64'hFFFFFFFFFFFFFFFF,
    parameter STACK_RESET_VALUE = 64'h0
) (
    input logic [63:0] g0_in, input logic [63:0] g1_in,
    input logic [63:0] g2_in, input logic [63:0] g3_in,
    input logic [63:0] g4_in, input logic [63:0] g5_in,
    input logic [63:0] g6_in, input logic [63:0] g7_in,
    input logic [63:0] g8_in, input logic [63:0] g9_in,
    input logic [63:0] g10_in, input logic [63:0] g11_in,
    input logic [63:0] g12_in, input logic [63:0] g13_in,
    input logic [63:0] g14_in, input logic [63:0] g15_in,
    input logic [63:0] fp_in, input logic [63:0] sp_in,

    output logic [63:0] g0, output logic [63:0] g1,
    output logic [63:0] g2, output logic [63:0] g3,
    output logic [63:0] g4, output logic [63:0] g5,
    output logic [63:0] g6, output logic [63:0] g7,
    output logic [63:0] g8, output logic [63:0] g9,
    output logic [63:0] g10, output logic [63:0] g11,
    output logic [63:0] g12, output logic [63:0] g13,
    output logic [63:0] g14, output logic [63:0] g15,
    output logic [63:0] fp, output logic [63:0] sp,
    input wire clk,
    input wire reset
);
    always @(posedge clk) begin
        if (reset) begin
            g0 <= GP_RESET_VALUE;
            g1 <= GP_RESET_VALUE;
            g2 <= GP_RESET_VALUE;
            g3 <= GP_RESET_VALUE;
            g4 <= GP_RESET_VALUE;
            g5 <= GP_RESET_VALUE;
            g6 <= GP_RESET_VALUE;
            g7 <= GP_RESET_VALUE;
            g8 <= GP_RESET_VALUE;
            g9 <= GP_RESET_VALUE;
            g10 <= GP_RESET_VALUE;
            g11 <= GP_RESET_VALUE;
            g12 <= GP_RESET_VALUE;
            g13 <= GP_RESET_VALUE;
            g14 <= GP_RESET_VALUE;
            g15 <= GP_RESET_VALUE;
            fp  <= STACK_RESET_VALUE;
            sp  <= STACK_RESET_VALUE;
        end
        else begin
            g0 <= g0_in;
            g1 <= g1_in;
            g2 <= g2_in;
            g3 <= g3_in;
            g4 <= g4_in;
            g5 <= g5_in;
            g6 <= g6_in;
            g7 <= g7_in;
            g8 <= g8_in;
            g9 <= g9_in;
            g10 <= g10_in;
            g11 <= g11_in;
            g12 <= g12_in;
            g13 <= g13_in;
            g14 <= g14_in;
            g15 <= g15_in;
            fp  <= fp_in;
            sp  <= sp_in;
        end
    end
endmodule
