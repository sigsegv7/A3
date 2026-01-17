//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module regbank #(
    parameter GP_RESET_VALUE = 64'hFFFFFFFFFFFFFFFF,
    parameter STACK_RESET_VALUE = 64'h0
) (
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
    end
endmodule
