#include <qb.h>

float phase = 0;

// Звезды
void starfeld() {

    srand(1);

    // Рисовать звездное небо
    for (int i = 0; i < 300; i++) {
        pset(rand() % 320, rand() % 200, rand());
    }
}

// Солнце
void circ(int x, int y, int rad) {

    for (int i = -rad; i < rad; i++)
    for (int j = -rad; j < rad; j++) {

        int r = i*i + j*j;
        if (r < rad*rad) {

            int b = point(x + j, y + i);
            int k = b + 110 + rad*16 - r;
            if (k < 0) k = 0;

            k += b;
            if (k > 255) k = 255;

            pset(x + j, y + i, k);
        }
    }
}

// Луна
void moon() {

    for (int i = -50; i < 50; i++)
    for (int j = -50; j < 50; j++) {
        if (i*i + j*j < 31*31) {
            pset(160 + j, 100 + i, 0);
        }
    }
}

program(13) palette_gray(); fps {

    cls(0);

    // Звезды
    starfeld();

    // Солнце и его изменчивость
    circ(160, 100, 50);
    circ((160. + 3.*sin(1.0*phase)), 100. + 1.*cos(1.0*phase), 54);
    circ((160. - 2.*sin(1.2*phase)), 100. - 2.*cos(0.9*phase), 55);
    circ((160. + 1.*sin(1.1*phase)), 100. - 3.*cos(1.2*phase), 56);

    // Симулятор Луны
    moon();

    // Надпись на заборе
    locate(9, 9); color(64 ); print("Moon Adventure");
    locate(8, 8); color(200); print("Moon Adventure");

    phase += 0.05;

} end

