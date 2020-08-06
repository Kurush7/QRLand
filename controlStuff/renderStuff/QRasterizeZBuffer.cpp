//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

std::mutex pixel_lock;

// todo: крутить куб, залезает за край - с другого краю че-то рисуется
// todo: недорисовка, полоски
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
    drawWedge(p0[0], p0[1], p1[0], p1[1], p2[0], p2[1], c, p0[2], p1[2], p2[2]);
}

inline bool belongs(int val, int a, int b) {
    return val > 0 || (val == 0 && (a > 0 || (a == 0 && b > 0)));
}

inline float triangle_area (float x1, float y1, float x2, float y2, float x3, float y3) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

void QRasterizeZBuffer::drawWedge(float xwf, float ywf, float xlf, float ylf, float xrf, float yrf, QRColor c,
                                  float zw, float zl, float zr) {
    int xl = QRound(xlf), xr = QRound(xrf);
    int xw = QRound(xwf), yw = QRound(ywf);
    int yl = QRound(ylf), yr = QRound(yrf);

    float S0 = fabs(triangle_area(xwf, ywf, xlf, ylf, xrf, yrf));
    if (S0 < QREPS_MINI || triangle_area(xw, yw, xl, yl, xr, yr) == 0) return;

    float la = ywf - ylf, ra = ywf - yrf, l2a = yrf-ylf;
    float lb = xlf - xwf, rb = xrf - xwf, l2b= xlf-xrf;
    float lc = -la*xwf - lb*ywf, rc = -ra*xwf - rb*ywf, l2c= -l2a*xlf - l2b*ylf;

    float x_test = (xlf + xrf )/2, y_test = (ylf+yrf)/2;
    x_test = (x_test+xwf)/2, y_test= (y_test+ywf)/2;
    if (la*x_test+lb*y_test+lc < -QREPS) la = -la, lb = -lb, lc = -lc;
    if (ra*x_test+rb*y_test+rc < -QREPS) ra = -ra, rb = -rb, rc = -rc;
    if (l2a*x_test+l2b*y_test+l2c < -QREPS) l2a = -l2a, l2b = -l2b, l2c = -l2c;

    int x=x_test, y=min(min(yl,yr), yw);
    int pos = y*w, ymax = max(max(yl,yr),yw);
    if (la < 0) swap(la, ra), swap(lb, rb), swap(lc, rc);
    if (la < 0) swap(la, l2a), swap(lb, l2b), swap(lc, l2c);
    if (ra > 0) swap(ra, l2a), swap(rb, l2b), swap(rc, l2c);
    // 3line = 1: left; -1 - right
    if (la < 0 || ra > 0) return;
    char third_line = fabs(l2a) < QREPS_MINI ? 0 : 1;
    if (third_line && l2a < 0) third_line = -1;

    float lval0, rval0, l2val0;
    int x0;
    float b10, b20, b1, b2;
    float z;

    float lval = la*x + lb*y + lc, rval = ra*x + rb*y + rc, l2val = l2a*x + l2b*y + l2c;
    b1 = triangle_area(xwf, ywf, xlf, ylf, x, y);
    b2 = triangle_area(xwf, ywf, xrf, yrf, x, y);
    float dyl = ywf-ylf, dyr=ywf-yrf, dxl=xlf-xwf, dxr=xrf-xwf;
    for(; y <= ymax; ++y) {
        x0 = x, b10 = b1, b20 = b2;
        //cout << y << "| ";
        lval0=lval, rval0=rval; l2val0=l2val;
        while (rval > -QREPS_MINI && (third_line!=-1 || l2val > -QREPS_MINI)) {
            //if ((lval > QREPS || (fabs(lval) < QREPS && (la > QREPS || (fabs(la) < QREPS && lb > QREPS)))) &&
            //    (rval > QREPS || (fabs(rval) < QREPS && (ra > QREPS || (fabs(ra) < QREPS && rb > QREPS))))) {
            if (lval > -QREPS_MINI && rval > -QREPS_MINI && l2val > -QREPS_MINI) {
                z = (fabs(b1)*zr + fabs(b2)*zl + (S0 - fabs(b1) - fabs(b2))*zw) / S0;
                if (z - zbuf[pos+x] > QREPS) {
                    //pixel_lock.lock();
                    img->setPixel(x, y, c);
                    //cout << x << ' ';
                    zbuf[pos+x] = z;
                    //pixel_lock.unlock();
                }
            }
            lval += la, rval += ra; l2val += l2a;
            x++;
            b1 += dyl, b2 += dyr;
        }
        //cout << " *** ";
        x = x0;
        b1 = b10, b2 = b20;
        lval = lval0, rval=rval0, l2val=l2val0;
        while (lval > -QREPS_MINI && (third_line!=1 || l2val > -QREPS_MINI)) {
            //if ((lval > QREPS || (fabs(lval) < QREPS && (la > QREPS || (fabs(la) < QREPS && lb > QREPS)))) &&
            //    (rval > QREPS || (fabs(rval) < QREPS && (ra > QREPS || (fabs(ra) < QREPS && rb > QREPS))))) {
            if (lval > -QREPS_MINI && rval > -QREPS_MINI && l2val > -QREPS_MINI) {
                z = (fabs(b1)*zr + fabs(b2)*zl + (S0 - fabs(b1) - fabs(b2))*zw) / S0;
                if (z - zbuf[pos+x] > QREPS) {
                    //pixel_lock.lock();
                    img->setPixel(x, y, c);
                    //cout << x << ' ';
                    zbuf[pos+x] = z;
                    //pixel_lock.unlock();
                }
            }
            lval -= la, rval -= ra; l2val -= l2a;
            x--;
            b1 -= dyl, b2 -= dyr;
        }
        pos += w;
        lval += lb, rval += rb, l2val += l2b;
        b1 += dxl, b2 += dxr;
        //cout << '\n';
    }
    //cout << "\n\n";
}


void QRasterizeZBuffer::clearBuf() {
    int sz = w*h;
    for (int i = 0; i < sz; i+= h)
        memcpy(&zbuf[i], row_example, sizeof(float)*w);

    memset(img->getData(), 0, w*h*4);
}