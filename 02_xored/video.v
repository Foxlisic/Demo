/* verilator lint_off WIDTH */
module video
(
    input               clock,
    output  reg         r,
    output  reg         g,
    output  reg         b,
    output              hs,
    output              vs,
    input   [3:0]       key
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
reg  [ 9:0] X    = 0;
reg  [ 8:0] Y    = 0;
// ---------------------------------------------------------------------
reg  [ 4:0] K;
reg  [ 9:0] A;
reg  [ 7:0] B;          // От 1 до 200
reg  [ 9:0] C;          // Результат от 1..1023
reg  [ 7:0] rx, ry;     // Значение X/Y и Y
reg         tx, ty;     // Сетка (шахматка 3D)
// ---------------------------------------------------------------------
wire [ 5:0] txy = {Y[2:0], X[2:0]};
wire [ 5:0] pattern =
    // ---
    txy == 8'h00 ? 0  : txy == 8'h01 ? 32 : txy == 8'h02 ?  8 : txy == 8'h03 ? 40 :
    txy == 8'h04 ?  2 : txy == 8'h05 ? 34 : txy == 8'h06 ? 10 : txy == 8'h07 ? 42 :
    txy == 8'h08 ? 48 : txy == 8'h09 ? 16 : txy == 8'h0A ? 56 : txy == 8'h0B ? 24 :
    txy == 8'h0C ? 50 : txy == 8'h0D ? 18 : txy == 8'h0E ? 58 : txy == 8'h0F ? 26 :
    // ---
    txy == 8'h10 ? 12 : txy == 8'h11 ? 44 : txy == 8'h12 ? 4  : txy == 8'h13 ? 36 :
    txy == 8'h14 ? 14 : txy == 8'h15 ? 46 : txy == 8'h16 ? 6  : txy == 8'h17 ? 38 :
    txy == 8'h18 ? 60 : txy == 8'h19 ? 28 : txy == 8'h1A ? 52 : txy == 8'h1B ? 20 :
    txy == 8'h1C ? 62 : txy == 8'h1D ? 30 : txy == 8'h1E ? 54 : txy == 8'h1F ? 22 :
    // ---
    txy == 8'h20 ? 3  : txy == 8'h21 ? 35 : txy == 8'h22 ? 11 : txy == 8'h23 ? 43 :
    txy == 8'h24 ? 1  : txy == 8'h25 ? 33 : txy == 8'h26 ? 9  : txy == 8'h27 ? 41 :
    txy == 8'h28 ? 51 : txy == 8'h29 ? 19 : txy == 8'h2A ? 59 : txy == 8'h2B ? 27 :
    txy == 8'h2C ? 49 : txy == 8'h2D ? 17 : txy == 8'h2E ? 57 : txy == 8'h2F ? 25 :
    // ---
    txy == 8'h30 ? 15 : txy == 8'h31 ? 47 : txy == 8'h32 ? 7  : txy == 8'h33 ? 39 :
    txy == 8'h34 ? 13 : txy == 8'h35 ? 45 : txy == 8'h36 ? 5  : txy == 8'h37 ? 37 :
    txy == 8'h38 ? 63 : txy == 8'h39 ? 31 : txy == 8'h3A ? 55 : txy == 8'h3B ? 23 :
    txy == 8'h3C ? 61 : txy == 8'h3D ? 29 : txy == 8'h3E ? 53 : 21;
// ---------------------------------------------------------------------

// Вывод видеосигнала
always @(posedge clock) begin

    // Кадровая развертка
    X <= xmax ?         0 : X + 1;
    Y <= xmax ? (ymax ? 0 : Y + 1) : Y;
    K <= xmax && ymax ? K + 1 : K;

    {r, g, b} <= 3'b000;

    // Вычисление текстуры XOR
    if (rx >= ry) begin rx <= rx + 1 - ry; tx <= tx + 1; end
    else          begin rx <= rx + 1; end

    // ----------------
    // Инициализация [на первой точки вывода]
    if (X == hzb) begin

        A <= 320;
        B <= Y - (199 + vtb);
        C <= 0;

    end
    // Запись результата [перед тем как начать выводить на экран]
    else if (X == hzb-1) begin

        tx <= C[0];
        ty <= C[0];
        rx <= (B - 1) - A;
        ry <= B;

    end
    // Деление числа A : B => C (остаток A)
    else begin if (A >= B) begin A <= A - B; C <= C + 1; end end
    // ----------------

    // Вывод окна видеоадаптера
    if (X >= hzb && X < hzb+hzv && Y >= vtb && Y < vtb+vtv)
    begin {r, g, b} <= Y > (200 + vtb) && (tx ^ ty) ? (B >= pattern + 24 ? 3'b111 : 3'b000) : 3'b000; end

end

endmodule
