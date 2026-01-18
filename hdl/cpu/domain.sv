//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module domain (
    input wire clk,
    input wire reset,
    output wire [7:0] alu_result
);

    logic [63:0] g0;  logic [63:0] g1;
    logic [63:0] g2;  logic [63:0] g3;
    logic [63:0] g4;  logic [63:0] g5;
    logic [63:0] g6;  logic [63:0] g7;
    logic [63:0] g8;  logic [63:0] g9;
    logic [63:0] g10; logic [63:0] g11;
    logic [63:0] g12; logic [63:0] g13;
    logic [63:0] g14; logic [63:0] g15;
    logic [63:0] fp;  logic [63:0] sp;

    alu alu (
        .operand_0(0),
        .operand_1(0),
        .opcode(0),
        .result(alu_result)
    );

    regbank rbk (
        .g0_in(g0), .g1_in(g1),
        .g2_in(g2), .g3_in(g3),
        .g4_in(g4), .g5_in(g5),
        .g6_in(g6), .g7_in(g7),
        .g8_in(g8), .g9_in(g9),
        .g10_in(g10), .g11_in(g11),
        .g12_in(g12), .g13_in(g13),
        .g14_in(g14), .g15_in(g15),
        .fp_in(fp), .sp_in(sp),
        .g0(g0),   .g1(g1),
        .g2(g2),   .g3(g3),
        .g4(g4),   .g5(g5),
        .g6(g6),   .g7(g7),
        .g8(g8),   .g9(g9),
        .g10(g10), .g11(g11),
        .g12(g12), .g13(g13),
        .g14(g14), .g15(g15),
        .sp(sp),   .fp(fp),
        .reset(reset),
        .clk(clk)
    );
endmodule
