//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

module dbgctl #(
    // UTAP select domains
    parameter UTAP_PC = 16'h01
) (
    input wire clk,

    // Select lines
    input wire [2:0] sel,

    // Board's TX, we get data in
    input logic dbg_rx,

    // Board's RX, we send data out
    output logic dbg_tx,

    // Control lines
    output wire pc_inhibit_out
);
    /* verilator lint_off UNUSEDSIGNAL */
    logic [15:0] sel_reg;
    logic [15:0] shift_reg;
    logic [7:0] n_bits;

    always @(posedge clk) begin
        /* verilator lint_off WIDTHEXPAND */
        sel_reg <= sel[2:0];
        if (sel_reg != 0) begin
            shift_reg <= {shift_reg[14:0], dbg_rx};
            n_bits <= n_bits + 1;
        end

        if (n_bits == 16) begin
            case (sel_reg)
                UTAP_PC: pc_inhibit_out <= shift_reg[0];
                default: ;
            endcase

            dbg_tx <= 0;
            n_bits <= 0;
            shift_reg <= 0;
        end
    end
endmodule
