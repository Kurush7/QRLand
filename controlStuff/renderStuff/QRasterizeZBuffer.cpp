//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

std::mutex pixel_lock;

// todo kinda brezenham?
void QRasterizeZBuffer::draw(Vector3D *_poly, int size, const Vector3D &norm, const QRTexture *texture) {
    renderData data;
    data.texture = texture;
    data.poly = _poly;
    data.n = size;
    if (data.n < 3) return;
    data.c = texture->getColor();
    colorManager->lightenColor(norm, data.c);   // todo not acceptable for mapping, so....

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

void QRasterizeZBuffer::fillRow(renderData &data) {
    float z = data.zl;
    float dz = (data.zr - data.zl) / (data.xri - data.xli + 0.);
    for (int x = data.xli; x <= data.xri; ++x) {
        if (z - zbuf[x][data.y] < -QREPS) {
            pixel_lock.lock();
            img->setPixel(x, data.y, data.c);
            zbuf[x][data.y] = z;
            pixel_lock.unlock();
        }
        z += dz;
    }
}

void QRasterizeZBuffer::clearBuf() {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            zbuf[i][j] = QRINF;
    }
    uchar* data = img->getData();
    auto x =  w*h*4;
    for (int i = 0; i < x; ++i)
        data[i] = 0;
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