//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

// todo kinda brezenham?
void QRasterizeZBuffer::draw(Vector3D *_poly, int size, const Vector3D &norm) {
    poly = _poly;
    n = size;
    if (n < 3) return;
    c = colorManager->getColor(norm);   // todo not acceptable for mapping, so....

    // needed right, but y = -y in our image-axis, so...
    dir = isRightRotate(poly[0], poly[1], poly[2])? -1 : 1;

    ll = 0, rr = -1;
    int maxY = -1;
    int myI = 0;
    correctFlag = false;
    for (int i = 0; i < n; ++i) {
        // some hardcode: digital len of(-50,50) is 101,
        // yet float's is 100. so translation after cutting becomes a bit dull....
        poly[myI][0] = max(0, min(w-1, QRound(poly[i][0])));
        //if (max(min(poly[myI][0], w-1), 0) != poly[myI][0]) correctFlag=true;
        poly[myI][1] = max(0, min(h-1, QRound(poly[i][1])));
        // used for: 2 points after cutting and discreting being the same (to avoid nan as 0/0 in bl, br)
        // todo not accurate z-mapping possible.... and inevitable
        if (i && poly[i][0] == poly[i-1][0] && poly[i][1] == poly[i-1][1]) continue;

        maxY = max((int)poly[myI][1], maxY);
        if (poly[myI][1] < poly[ll][1])
            ll = rr = myI;
        else if (poly[myI][1] == poly[ll][1]) {
            if (poly[myI][0] < poly[ll][0]) rr = ll, ll = myI;
            else rr = myI;
        }
        myI++;
    }
    if (poly[0][0] == poly[myI-1][0] && poly[0][1] == poly[myI-1][1]) myI--;
    n = myI;
    if (n == 0) return;

    if (rr == -1) rr = ll;
    jumpL();
    jumpR();
    xl = poly[left][0] , xr = poly[right][0];
    y = poly[left][1];

    while(y < maxY) {
        xli = QRound(xl);
        xri = QRound(xr);
        fillRow();
        y++;
        if (y > poly[ll][1]) {
            if (y > poly[rr][1]) {
                jumpR();
            }
            jumpL();
        }
        else if (y > poly[rr][1]) jumpR();

        xl -= bl;   // ax+by+c=0 -> x=-c/a +y*(-b/a)
        xr -= br;
        zl += dzl;
        zr += dzr;
    }
}

void QRasterizeZBuffer::fillRow() {
    if (xli == xri) return;
    /*if (correctFlag) {
        xli = max(min(xli, w-1), 0);
        xri = max(min(xri, w-1), 0);
        y = max(min(y, h-1), 0);
    }*/
    float z = zl;
    float dz = (zr - zl + 0.) / (xri - xli + 0.);
    for (int x = xli; x <= xri; ++x) {
        if (z - zbuf[x][y] < -QREPS) {
            img->setPixel(x, y, c);
            zbuf[x][y] = z;
        }
        /*else if (fabs(z-zbuf[x][y]) < QREPS) {
            img->mixPixel(x, y, c);
        }*/
        z += dz;
    }
}

void QRasterizeZBuffer::clearBuf() {
    c = QRColor("black");
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            zbuf[i][j] = QRINF;
    }
    uchar* data = img->getData();
    auto x =  w*h*4;
    for (int i = 0; i < x; ++i)
        data[i] = 0;
}

inline void QRasterizeZBuffer::jumpL() {
    left = ll;
    ll = (left-dir + n) % n;    // todo not accurate?
    if (poly[left][1] - poly[ll][1] == 0)
        bl = 0, dzl = 0;    // bug hiding)
    else {
        bl = (poly[ll][0] - poly[left][0]) / (poly[left][1] - poly[ll][1] + 0.);
        zl = poly[left][2];
        dzl = (poly[ll][2] - zl) / (poly[ll][1] - poly[left][1]+0.);
    }
}
inline void QRasterizeZBuffer::jumpR() {
    right = rr;
    rr = (right+dir + n) % n;   // todo not accurate?
    if (poly[right][1] - poly[rr][1] == 0)
        br = 0, dzr = 0;    // bug hiding)
    else {
        br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1] + 0.);
        zr = poly[right][2];
        dzr = (poly[rr][2] - zr) / (poly[rr][1] - poly[right][1] + 0.);
    }
}