#include <qb.h>

float rot = 0;

// -----------------------------------------------------------------------------

// Псевдослучайный шум
double rnd(double x, double y) {

    double m = sin(x * 12.9898 + y * 78.233) * 43758.54531229988;
    return m - floor(m);
}

// Просто шум в точке
double noise(float x, float y) {

    double ix = floor(x);
    double iy = floor(y);
    double fx = x - ix;
    double fy = y - iy;

    double a = rnd(ix,      iy);
    double b = rnd(ix + 1., iy);
    double c = rnd(ix,      iy + 1.);
    double d = rnd(ix + 1., iy + 1.);

    double ux = fx * fx * (3 - 2 * fx);
    double uy = fy * fy * (3 - 2 * fy);

    // a b Интерполяция по 4-м точкам
    // c d Используя fx, fy в качестве позиции fx,fy=[0,1]
    return a * (1 - ux) +
           b * ux +
           (c - a) * uy * (1 - ux) +
           (d - b) * ux * uy;
}

// Фрактальный шум
double fbm (double x, double y) {

    double value  = 0;
    double amp    = .5;
    double freq   = 0;

    for (int i = 0; i < 5; i++) {

        value += amp * noise(x, y);
        x = 2.*x;
        y = 2.*y;
        amp = amp * .5;
    }

    return value;
}

// -----------------------------------------------------------------------------

// Нормализация вектора
vec3 normalize (vec3 c) {

    double d = sqrt(c.x*c.x + c.y*c.y + c.z*c.z);

    d   = d ?: 1;
    c.x = c.x / d;
    c.y = c.y / d;
    c.z = c.z / d;

    return c;
};

// Вычисление пересечений луча D со сферой в точке O и радиусом R
double sphere (vec3 d, vec3 o, double r) {

    double sp   = -1;
    double a    = d.x * d.x + d.y * d.y + d.z * d.z;
    double b    = -2 * (d.x * o.x + d.y * o.y + d.z * o.z);
    double c    = o.x * o.x + o.y * o.y + o.z * o.z - r;
    double det  = b*b - 4*a*c;

    if (det >= 0) {

        det = sqrt(det);
        double x1 = (-b - det) / (2 * a);
        double x2 = (-b + det) / (2 * a);

        if (x1 < 0 && x2 < 0) sp = -1;
        if (x1 < 0 && x2 > 0) sp = x2;
        if (x1 > 0 && x2 < 0) sp = x1;
        if (x1 > 0 && x2 > 0 && x1 < x2) sp = x1;
        if (x1 > 0 && x2 > 0 && x1 >= x2) sp = x2;
    }

    return sp;
}

// Кастомная палитра
void custompal() {

    FOR(i,1,63) palette(   i, rgb(i,      128 + i * 2, 4*i));   // Earth [0..63]
    FOR(i,0,7)  palette(64+i, rgb(i * 32, 32 + 24 * i, 255));   // Water [64..71]
    FOR(i,0,63) palette(72+i, rgb(4*i,    4*i,         4*i));   // Starfeld [72..135]
}

// -----------------------------------------------------------------------------

program(13) custompal(); do {

    double u, v, m, dt = 8;
    vec3 c, o = {0, 0, 1.5}, sun = normalize({1, 1, -.5});

    srand(1);

    // Звездное небо
    FOR(i,0,320) pset(rand()%320, rand()%200, rand()%64+72);

    // Приветствие
    color(120); locate(8, 8); print("Earth Song");

    // Планетарность
    FOR (y,-100,99) FOR (x,-160,159) {

        c = {(float)x / 100, (float)y / 100, 1};

        if ((m = sphere(c, o, 1)) > 0) {

            // Точка пересечения
            c.x = c.x * m - o.x;
            c.y = c.y * m - o.y;
            c.z = c.z * m - o.z;
            c   = normalize(c);

            // Вычислить UV
            u = atan2(c.z, c.x);
            v = atan2(c.z, c.y);
            u = u + rot;

            // Получение дробной части
            u = u - floor(u);
            v = v - floor(v);
            m = fbm(dt * u, dt * v) * 63;

            // Свет Солнца
            int dl = 128*(c.x * sun.x + c.y * sun.y + c.z * sun.z);

            // Дизеринг
            dl = dl + lookupdith[x&7][y&7] - 64;

            // Вода или поверхность?
            m = (m <= 32) ? 64 : (2*m - 63);

            // Использовать дизеринг для затененения
            pset(160 + x, 100 - y, dl <= 0 ? 0 : m);
        }
    }

    rot += 0.005;

} fps end

