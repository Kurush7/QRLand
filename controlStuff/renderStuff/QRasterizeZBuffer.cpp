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
    if (data.n < 3) return;
    data.c = texture->getColor();
    colorManager->lightenColor(norm, data.c);   // todo not acceptable for mapping, so....
    if (data.n == 3)
    {
        drawTriangle(data);
        return;
    }
    return; //todo
    // needed right, but y = -y in our image-axis, so...
    data.dir = isRightRotate(data.poly[0], data.poly[1], data.poly[2])? -1 : 1;

    data.ll = 0, data.rr = -1;
    int maxY = -1;
    int myI = 0;
    for (int i = 0; i < data.n; ++i) {
        // some hardcode: digital len of(-50,50) is 101,
        // yet float's is 100. so translation after cutting becomes a bit dull....
        data.poly[myI][0] = max(0, min(w-1, QRound(data.poly[i][0])));
        //if (max(min(poly[myI][0], w-1), 0) != poly[myI][0]) correctFlag=true;
        data.poly[myI][1] = max(0, min(h-1, QRound(data.poly[i][1])));
        // used for: 2 points after cutting and discreting being the same (to avoid nan as 0/0 in bl, br)
        // todo not accurate z-mapping possible.... and inevitable
        if (i && data.poly[i][0] == data.poly[i-1][0] && data.poly[i][1] == data.poly[i-1][1]) continue;

        maxY = max((int)data.poly[myI][1], maxY);
        if (data.poly[myI][1] < data.poly[data.ll][1])
            data.ll = data.rr = myI;
        else if (data.poly[myI][1] == data.poly[data.ll][1]) {
            if (data.poly[myI][0] < data.poly[data.ll][0]) data.rr = data.ll, data.ll = myI;
            else data.rr = myI;
        }
        myI++;
    }
    if (data.poly[0][0] == data.poly[myI-1][0] && data.poly[0][1] == data.poly[myI-1][1]) myI--;
    data.n = myI;
    if (data.n == 0) return;

    if (data.rr == -1) data.rr = data.ll;
    jumpL(data);
    jumpR(data);
    data.xl = data.poly[data.left][0] , data.xr = data.poly[data.right][0];
    data.y = data.poly[data.left][1];

    while(data.y < maxY) {
        data.xli = QRound(data.xl);
        data.xri = QRound(data.xr);
        fillRow(data);
        data.y++;
        if (data.y > data.poly[data.ll][1]) jumpL(data);
        if (data.y > data.poly[data.rr][1]) jumpR(data);

        data.xl -= data.bl;   // ax+by+c=0 -> x=-c/a +y*(-b/a)
        data.xr -= data.br;
        data.zl += data.dzl;
        data.zr += data.dzr;
    }
}

void QRasterizeZBuffer::drawTriangle(renderData &data) {
    int p0x=QRound(data.poly[0][0]), p0y=QRound(data.poly[0][1]),
        p1x=QRound(data.poly[1][0]), p1y=QRound(data.poly[1][1]),
        p2x=QRound(data.poly[2][0]), p2y=QRound(data.poly[2][1]);

    if (p1y < p0y) swap(p0y, p1y), swap(p0x, p1x), swap(data.poly[0][2], data.poly[1][2]);
    if (p2y < p0y) swap(p0y, p2y), swap(p0x, p2x), swap(data.poly[0][2], data.poly[2][2]);
    if (p2y > p1y) swap(p1y, p2y), swap(p1x, p2x), swap(data.poly[1][2], data.poly[2][2]);

    if (p0y == p2y)
        drawWedge(p1x, p1y, p0y, p0x, p2x, data.c,data.poly[1][2], data.poly[0][2], data.poly[2][2]);
    else if (p1y == p2y)
        drawWedge(p0x, p0y, p1y, p1x, p2x, data.c,data.poly[0][2], data.poly[1][2], data.poly[2][2]);
    else {
        float x_new = p0x + (p2y-p0y+0.f) * (p1x-p0x + 0.f) / (p1y - p0y+0.f);
        float z_new;
        if (fabs(x_new - p1x) < QREPS) z_new = data.poly[1][2];
        else z_new = data.poly[0][2] + (data.poly[1][2]- data.poly[0][2]) * (x_new - p0x+0.f) / (p1x - p0x + 0.f);

        drawWedge(p0x, p0y, p2y, p2x, x_new, data.c,data.poly[0][2], data.poly[2][2], z_new);
        drawWedge(p1x, p1y, p2y, p2x, x_new, data.c,data.poly[1][2], data.poly[2][2], z_new);
    }
}

inline bool belongs(int val, int a, int b) {
    return val > 0 || (val == 0 && (a > 0 || a == 0 && b > 0));
}

inline int triangle_area (int x1, int y1, int x2, int y2, int x3, int y3) {
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
    float lval, rval;
    float z;

    float S0 = triangle_area(xw, yw, xl, y_floor, xr, y_floor);
    if (S0 == 0) return;
    float b1,b2;

    int ymin = min(yw, y_floor), ymax = max(yw, y_floor);
    int xmin = min(min(xl, xr), xw), xmax = max(max(xl, xr), xw);
    int pos = ymin*w;

    for (int y = ymin; y <= ymax; ++y) {
        lval = la * xmin + lb * y + lc;
        rval = ra * xmin + rb * y + rc;
        for (int x = xmin; x <= xmax; ++x) {
            if ((lval > QREPS || (fabs(lval) < QREPS && (la > QREPS || (fabs(la) < QREPS && lb > QREPS)))) &&
                (rval > QREPS || (fabs(rval) < QREPS && (ra > QREPS || (fabs(ra) < QREPS && rb > QREPS))))) {

                //b1 = triangle_area(xw, yw, xl, y_floor, x, y) / S0;
                //b2 = triangle_area(xw, yw, xr, y_floor, x, y) / S0;
                b1 = abs((xl - xw) * (y - yw) - (y_floor - yw) * (x - xw)) / S0;
                b2 = abs((xr - xw) * (y - yw) - (y_floor - yw) * (x - xw)) / S0;
                z = b1*zr + b2*zl + (1 - b1 - b2)*zw;
                if (z - zbuf[pos+x] > QREPS) {
                    pixel_lock.lock();
                    //cout << ' ';
                    img->setPixel(x, y, c);
                    zbuf[pos+x] = z;
                    pixel_lock.unlock();
                }
            }
            lval += la, rval += ra;
        }
        pos += w;
    }

}

void QRasterizeZBuffer::fillRow(renderData &data) {
    float z = data.zl;
    float dz = (data.zr - data.zl) / (data.xri - data.xli + 0.);
    int pos = data.y*w;
    for (int x = data.xli; x <= data.xri; ++x) {
        if (z - zbuf[pos+x] > -QREPS) {
            pixel_lock.lock();
            img->setPixel(x, data.y, data.c);
            zbuf[pos+x] = z;
            pixel_lock.unlock();
        }
        z += dz;
    }
}

void QRasterizeZBuffer::clearBuf() {
    int sz = w*h;
    for (int i = 0; i < sz; i+= h)
        memcpy(&zbuf[i], row_example, sizeof(float)*w);

    memset(img->getData(), 0, w*h*4);
}

inline void QRasterizeZBuffer::jumpL(renderData &data) {
    data.left = data.ll;
    data.ll = (data.left-data.dir + data.n) % data.n;    // todo not accurate?
    if (data.poly[data.left][1] - data.poly[data.ll][1] == 0)
        data.bl = 0, data.dzl = 0;    // bug hiding)
    else {
        data.bl = (data.poly[data.ll][0] - data.poly[data.left][0]) / (data.poly[data.left][1] - data.poly[data.ll][1] + 0.);
        data.zl = data.poly[data.left][2];
        data.dzl = (data.poly[data.ll][2] - data.zl) / (data.poly[data.ll][1] - data.poly[data.left][1]+0.);
    }
}
inline void QRasterizeZBuffer::jumpR(renderData &data) {
    data.right = data.rr;
    data.rr = (data.right+data.dir + data.n) % data.n;
    if (data.poly[data.right][1] - data.poly[data.rr][1] == 0)
        data.br = 0, data.dzr = 0;
    else {
        data.br = (data.poly[data.rr][0] - data.poly[data.right][0]) / (data.poly[data.right][1] - data.poly[data.rr][1] + 0.);
        data.zr = data.poly[data.right][2];
        data.dzr = (data.poly[data.rr][2] - data.zr) / (data.poly[data.rr][1] - data.poly[data.right][1] + 0.);
    }
}