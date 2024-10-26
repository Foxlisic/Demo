module max2
(
    input               clock,
    input       [3:0]   key,
    output reg  [7:0]   led,
    inout               dp,
    inout               dn,
    inout               pt
);

// Генератор 25 Мгц
reg [1:0] div; always @(posedge clock) div <= div + 1;

video VIDEO
(
    .clock      (div[1]),   // 25 MHZ
    .r          (led[0]),
    .g          (led[3]),
    .b          (led[4]),
    .hs         (led[1]),
    .vs         (led[2]),
    .key        (key)
);

endmodule

