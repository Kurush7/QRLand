//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

std::mutex pixel_lock;

// todo kinda brezenham... too bad?
void QRasterizeZBuffer::draw(Vector3D *_poly, int size, const Vector3D &norm, const QRTexture *texture) {
    renderData data;
    data.texture = texture;
    data.poly = _poly;
    data.n = size;
    data.c = texture->getColor();

    colorManager->lightenColor(norm, data.c);   // todo not acceptable for mapping, so....
    Vector3D p0 = data.poly[0];
    for (int i = 0; i < size - 2; ++i)
    {
        drawTriangle(p0, data.poly[1], data.poly[2], data.c);
        data.poly = &data.poly[1];
    }
}

void QRasterizeZBuffer::drawTriangle(const Vector3D &p0, const Vector3D &p1, const Vector3D &p2, QRColor c) {
    int p0x=QRound(p0.arr[0]), p0y=QRound(p0.arr[1]),
        p1x=QRound(p1.arr[0]), p1y=QRound(p1.arr[1]),
        p2x=QRound(p2.arr[0]), p2y=QRound(p2.arr[1]);
    float z0=p0.arr[2], z1=p1.arr[2], z2=p2.arr[2];
    if (p1y < p0y) swap(p0y, p1y), swap(p0x, p1x), swap(z0, z1);
    if (p2y < p0y) swap(p0y, p2y), swap(p0x, p2x), swap(z0, z2);
    if (p2y > p1y) swap(p1y, p2y), swap(p1x, p2x), swap(z1, z2);

    if (p0y == p2y)
        drawWedge(p1x, p1y, p0y, p0x, p2x, c, z1, z0, z2);
    else if (p1y == p2y)
        drawWedge(p0x, p0y, p1y, p1x, p2x, c, z0, z1, z2);
    else {
        float x_new = p0x + (p2y-p0y+0.f) * (p1x-p0x + 0.f) / (p1y - p0y+0.f);
        float z_new;
        if (fabs(x_new - p1x) < QREPS) z_new = z1;
        else z_new = z0 + (z1- z0) * (x_new - p0x+0.f) / (p1x - p0x + 0.f);
        drawWedge(p0x, p0y, p2y, p2x, x_new, c, z0, z2, z_new);
        drawWedge(p1x, p1y, p2y, p2x, x_new, c, z1, z2, z_new);
    }
}

inline bool belongs(int val, int a, int b) {
    return val > 0 || (val == 0 && (a > 0 || a == 0 && b > 0));
}

inline float triangle_area (int x1, int y1, int x2, int y2, int x3, int y3) {
    return abs((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
}

void QRasterizeZBuffer::drawWedge(int xw, int yw, int y_floor, int xl, float xrf, QRColor c,
        float zw, float zl, float zr) {
    float la = yw - y_floor, ra = yw - y_floor;
    float lb = xl - xw, rb = xrf - xw;
    float lc = -la*xw - lb*yw, rc = -ra*xw - rb*yw;

    float x_test = (xl + xrf) / 2;
    if (la*x_test+lb*y_floor+lc < -QREPS) la = -la, lb = -lb, lc = -lc;
    if (ra*x_test+rb*y_floor+rc < -QREPS) ra = -ra, rb = -rb, rc = -rc;

    int xr = QRound(xrf);
    float z;

    float S0 = triangle_area(xw, yw, xl, y_floor, xr, y_floor);
    if (S0 == 0) return;
    float b1,b2;

    int x, y, x0;
    if (yw > y_floor) x = xl, y = y_floor;
    else x = xw, y = yw;
    int pos = y*w;
    int ymax = max(yw, y_floor);
    if (la < 0) {
        swap(la, ra);
        swap(lb, rb);
        swap(lc, rc);
    }
    float lval = la * x + lb * y + lc, rval = ra * x + rb * y + rc;
    float lval0, rval0;
    for(; y !=ymax; ++y) {
        x0 = x;
        lval0=lval, rval0=rval;
        while (rval > -QREPS) {
            if ((lval > QREPS || (fabs(lval) < QREPS && (la > QREPS || (fabs(la) < QREPS && lb > QREPS)))) &&
                (rval > QREPS || (fabs(rval) < QREPS && (ra > QREPS || (fabs(ra) < QREPS && rb > QREPS))))) {
                b1 = triangle_area(xw, yw, xl, y_floor, x, y);
                b2 = triangle_area(xw, yw, xr, y_floor, x, y);
                z = (b1*zr + b2*zl + (S0 - b1 - b2)*zw) / S0;
                if (z - zbuf[pos+x] > QREPS) {
                    //pixel_lock.lock();
                    img->setPixel(x, y, c);
                    zbuf[pos+x] = z;
                    //pixel_lock.unlock();
                }
            }
            lval += la, rval += ra; x++;
        }
        x = x0;
        lval = lval0, rval=rval0;
        while (lval > -QREPS) {
            if ((lval > QREPS || (fabs(lval) < QREPS && (la > QREPS || (fabs(la) < QREPS && lb > QREPS)))) &&
                (rval > QREPS || (fabs(rval) < QREPS && (ra > QREPS || (fabs(ra) < QREPS && rb > QREPS))))) {
                b1 = triangle_area(xw, yw, xl, y_floor, x, y) / S0;
                b2 = triangle_area(xw, yw, xr, y_floor, x, y) / S0;
                z = b1*zr + b2*zl + (1 - b1 - b2)*zw;
                if (z - zbuf[pos+x] > QREPS) {
                    //pixel_lock.lock();
                    img->setPixel(x, y, c);
                    zbuf[pos+x] = z;
                    //pixel_lock.unlock();
                }
            }
            lval -= la, rval -= ra; x--;
        }
        pos += w;
        lval += lb, rval += rb;
        //cout << '\n';
    }
}


void QRasterizeZBuffer::clearBuf() {
    int sz = w*h;
    for (int i = 0; i < sz; i+= h)
        memcpy(&zbuf[i], row_example, sizeof(float)*w);

    memset(img->getData(), 0, w*h*4);
}