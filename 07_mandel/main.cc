#include <qb.h>

int cnt = 0;

int mandel(double cx, double cy) {

    double x = 0, y = 0;
    for (int i = 0; i < 256; i++) {

        double tx = x*x - y*y + cx,
               ty = 2*x*y     + cy;

        x = tx;
        y = ty;

        cnt++;

        if (x*x + y*y >= 4) return i;
    }

    return 255;
}

program(3)

// float x0 =  0.3, y0 = 0.0, fa = (1./200.)/(16.);
   float x0 = -0.5, y0 = 0.0, fa = (1./200.)/(1.);

cnt = 0;
for (int y = -200; y < 200; y++)
for (int x = -320; x < 320; x++) {

    int cl = mandel(x*fa + x0, y*fa + y0);
    pset(x + 320, y + 200, cl);
}

printf("D=%d\n", cnt);

fps;

end

