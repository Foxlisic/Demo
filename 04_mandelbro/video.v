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
    vtv =  400, vtf =   12, vts =    2, vtb =   35, vtw =  449;
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

// Запрошенный (x,y)
reg  [31:0] rx  = 32'b0_001_0000000000000000000000000000,
            ry  = 32'b0_001_0000000000000000000000000000;

// Текущее вычисляемое
reg  [31:0] cx = 32'b0_011_0000000000000000000000000000,
            cy = 32'b0_001_0000000000000000000000000000;

// 31 Sign 30-28 Int 27-0 Fract
wire [31:0] absx = cx[31] ? -cx : cx;
wire [31:0] absy = cy[31] ? -cy : cy;

// Вычисление умножений
wire [63:0] xx_ = absx*absx,
            yy_ = absy*absy,
            xy_ = absx*absy;
wire [31:0] xx  = xx_[58:28],
            yy  = yy_[58:28],
            xy  = cx[31] ^ cy[31] ? -xy_[58:28] : xy_[58:28];
wire [31:0] ac  = xx + yy,
            bc  = xy << 1;
// ---------------------------------------------------------------------

// Вывод видеосигнала
always @(posedge clock) begin

    // Кадровая развертка
    X <= xmax ?         0 : X + 1;
    Y <= xmax ? (ymax ? 0 : Y + 1) : Y;

    {r, g, b} <= 12'h000;

    // Вывод окна видеоадаптера
    if (show) begin {r, g, b} <= 12'h004; end

end

endmodule
