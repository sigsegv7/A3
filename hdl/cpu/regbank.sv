//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module regbank #(
    parameter GP_RESET_VALUE = 64'hFFFFFFFFFFFFFFFF,
    parameter STACK_RESET_VALUE = 64'h0,

    // Register values
    parameter REG_G0 = 6'h00,  parameter REG_G1  = 6'h01,
    parameter REG_G2 = 6'h02,  parameter REG_G3  = 6'h03,
    parameter REG_G4 = 6'h04,  parameter REG_G5  = 6'h05,
    parameter REG_G6 = 6'h06,  parameter REG_G7  = 6'h07,
    parameter REG_G8 = 6'h08,  parameter REG_G9  = 6'h09,
    parameter REG_G10 = 6'h0A, parameter REG_G11 = 6'h0B,
    parameter REG_G12 = 6'h0C, parameter REG_G13 = 6'h0D,
    parameter REG_G14 = 6'h0E, parameter REG_G15 = 6'h0F,
    parameter REG_FP = 6'h16,  parameter REG_SP  = 6'h17
) (
    // Register select, value and write enable line
    input logic  [5:0]  regsel,
    input logic  [63:0] reg_val,
    input wire   reg_we,

    input wire clk,
    input wire reset
);
    /* verilator lint_off UNUSEDSIGNAL */
    logic [63:0] g0;  logic [63:0] g1;
    logic [63:0] g2;  logic [63:0] g3;
    logic [63:0] g4;  logic [63:0] g5;
    logic [63:0] g6;  logic [63:0] g7;
    logic [63:0] g8;  logic [63:0] g9;
    logic [63:0] g10; logic [63:0] g11;
    logic [63:0] g12; logic [63:0] g13;
    logic [63:0] g14; logic [63:0] g15;
    logic [63:0] fp;  logic [63:0] sp;

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
            if (reg_we) begin
                case (regsel)
                    REG_G0: g0 <= reg_val;
                    REG_G1: g1 <= reg_val;
                    REG_G2: g2 <= reg_val;
                    REG_G3: g3 <= reg_val;
                    REG_G4: g4 <= reg_val;
                    REG_G5: g5 <= reg_val;
                    REG_G6: g6 <= reg_val;
                    REG_G7: g7 <= reg_val;
                    REG_G8: g8 <= reg_val;
                    REG_G9: g9 <= reg_val;
                    REG_G10: g10 <= reg_val;
                    REG_G11: g11 <= reg_val;
                    REG_G12: g12 <= reg_val;
                    REG_G13: g13 <= reg_val;
                    REG_G14: g14 <= reg_val;
                    REG_G15: g15 <= reg_val;
                    REG_FP: fp <= reg_val;
                    REG_SP: sp <= reg_val;
                    default: ;
                endcase
            end
        end
    end
endmodule
