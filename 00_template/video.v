/* verilator lint_off WIDTH */

module video
(
    input   wire        clock,
    output  reg [3:0]   r,
    output  reg [3:0]   g,
    output  reg [3:0]   b,
    output  wire        hs,
    output  wire        vs
);
// ---------------------------------------------------------------------
// Тайминги для горизонтальной и вертикальной развертки
parameter
//  Visible     Front       Sync        Back        Whole
    hzv =  640, hzf =   16, hzs =   96, hzb =   48, hzw =  800,
    vtv =  480, vtf =   10, vts =    2, vtb =   33, vtw =  525;
//  vtv =  400, vtf =   12, vts =    2, vtb =   35, vtw =  449;
// ---------------------------------------------------------------------
assign hs = X < (hzb + hzv + hzf);
assign vs = Y < (vtb + vtv + vtf);
// ---------------------------------------------------------------------
wire        xmax = (X == hzw - 1);
wire        ymax = (Y == vtw - 1);
wire        show = X >= hzb && X < hzb+hzv && Y >= vtb && Y < vtb+vtv;
reg  [10:0] X    = 0;
reg  [10:0] Y    = 0;
wire [ 9:0] x    = X - hzb;
wire [ 9:0] y    = Y - vtb;
// ---------------------------------------------------------------------

// Вывод видеосигнала
always @(posedge clock) begin

    // Кадровая развертка
    X <= xmax ?         0 : X + 1;
    Y <= xmax ? (ymax ? 0 : Y + 1) : Y;

    {r, g, b} <= 12'h000;

    // Вывод окна видеоадаптера
    if (show) {r, g, b} <= 12'h004;

end

endmodule
