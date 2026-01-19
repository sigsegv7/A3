//
// Copyright (c) 2026, Ian Moffett.
// Provided under the BSD-3 clause.
//

//
// Program counter module
//
module pc #(
    parameter CTL_READ_ADDR = 8'h02
) (
    input wire reset,
    input wire clk,
    input logic [7:0] bus_data_in,

    output logic [7:0] ctl_op_out,
    output logic [63:0] ctl_data_out
);
    logic [16:0] pc;

    always @(posedge clk) begin
        //
        // A3 spec states that upon reset, the program counter shall
        // be initialized to a value of zero.
        //
        if (reset) begin
            pc <= 0;
            ctl_op_out <= 0;
            ctl_data_out <= 0;
        end
        else begin
            /* verilator lint_off WIDTHEXPAND */
            ctl_op_out <= CTL_READ_ADDR;
            ctl_data_out <= pc[16:0];
        end

        case (bus_data_in)
            default: ;
        endcase

        pc <= pc + 4;
    end
endmodule
