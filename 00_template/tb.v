`timescale 10ns / 1ns
module tb;

// Тестбенчевые сигналы
// =============================================================================
reg reset_n;
reg clock_hi; always #0.5 clock_hi = ~clock_hi;
reg clock_25; always #2.0 clock_25 = ~clock_25;

reg [7:0] memory[65536];

initial begin

    // $readmemh("tb.hex", memory, 1'b0);
    $dumpfile("tb.vcd");
    $dumpvars(0, tb);

    clock_hi = 0;
    clock_25 = 0;
    reset_n  = 0;

    #3.0  reset_n = 1;
    #2000 $finish;

end

endmodule
