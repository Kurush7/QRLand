//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

// todo kinda brezenham?
void QRasterizeZBuffer::draw(Vector3D *_poly, int size, const Vector3D &norm) {
    poly = _poly;
    n = size;
    normal = norm;
    // todo poly of ints?
    // needed right, but y = -y, so...
    dir = isRightRotate(poly[0], poly[1], poly[2])? -1 : 1;

    ll = 0, rr = -1;
    int maxY = -1;
    for (int i = 0; i < n; ++i) {
        poly[i][0] = min(w-1, QRound(poly[i][0]));  // some hardcode: digital len of(-50,50) is 101,
        poly[i][1] = min(h-1, QRound(poly[i][1])); // yet double's is 100. so translation after cutting becomes a bit dull....
        maxY = max((int)poly[i][1], maxY);
        if (poly[i][1] < poly[ll][1])
            ll = rr = i;
        else if (poly[i][1] == poly[ll][1]) {
            if (poly[i][0] < poly[ll][0]) rr = ll, ll = i;
            else rr = i;
        }
    }

    //for (int i = 0; i < size; ++i)
    //    cout << poly[i] << ' ';
    //cout << '\n';

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
                //if (rr == ll) break;
                jumpR();
            }
            jumpL();
            //if (ll == right && left == rr) break;
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

    double z = zl;
    double dz = (zr - zl + 0.) / (xri - xli + 0.);

    for (int x = xli; x <= xri; ++x) {
        if (z - zbuf[x][y] < QREPS) {
            QRColor c = colorManager->getColor(normal);

            if (fabs(z - zbuf[x][y]) < QREPS) { // todo not needed
                auto cc = img->getPixel(x,y);   // todo too long
                c.r = (c.r + cc.r) / 2;
                c.g = (c.g + cc.g) / 2;
                c.b = (c.b + cc.b) / 2;
            }
            img->setPixel(x, y, c);
            zbuf[x][y] = z;
        }
        z += dz;
    }
}

void QRasterizeZBuffer::clearBuf() {
    // todo prepare data, then std::copy
    // or image to have fill method
    auto c = QRColor("black");
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            zbuf[i][j] = QRINF;
    }
    uchar* data = img->getData();
    for (int i = 0; i < w*h*4; ++i)
        data[i] = 0;
}

inline void QRasterizeZBuffer::jumpL() {
    left = ll;
    ll = (left-dir + n) % n;    // todo not accurate?
    bl = (poly[ll][0] - poly[left][0]) / (poly[left][1] - poly[ll][1] + 0.);
    zl = poly[left][2];
    dzl = (poly[ll][2] - zl) / (poly[ll][1] - poly[left][1]+0.);
}
inline void QRasterizeZBuffer::jumpR() {
    right = rr;
    rr = (right+dir + n) % n;   // todo not accurate?
    br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1] + 0.);
    zr = poly[right][2];
    dzr = (poly[rr][2] - zr) / (poly[rr][1] - poly[right][1]+0.);
}