//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

std::mutex pixel_lock;

// todo: крутить куб у-ахис: диаг-незакрас на среднем размере
//  !!! видны пятна задних областей - если выключен мьютекс !!!
// segfault: dy = 0.000000...., но при округлении они разносятся в разные стороны... бесконечный прирост
void QRasterizeZBuffer::draw(Vector3D *poly, int size, const Vector3D &norm, const QRTexture *texture) {
    auto c = texture->getColor();
    //colorManager->lightenColor(ZeroVector, norm, c);

    if (size == 3) drawTriangle(poly[0][0], poly[0][1], poly[0][2],
                                poly[1][0], poly[1][1], poly[1][2],
                                poly[2][0], poly[2][1], poly[2][2], c);
    else {
        Vector3D p0 = poly[0];
        for (int i = 0; i < size - 2; ++i) {
            drawTriangle(p0[0], p0[1], p0[2],
                         poly[1][0], poly[1][1], poly[1][2],
                         poly[2][0], poly[2][1], poly[2][2], c);
            poly = &poly[1];
        }
    }
}

void QRasterizeZBuffer::draw(float** points, int32_t* poly, int size, const Vector3D &norm, const QRTexture *texture) {
    auto c0 = texture->getColor();
    draw(points, poly, size, norm, c0);

}

void QRasterizeZBuffer::draw(float** points, int32_t* poly, int size, const Vector3D &norm, const QRColor &c0) {
    //colorManager->lightenColor(ZeroVector, norm, c0);

    // todo optimize
    if (size == 3) drawTriangle(points[poly[0]][0], points[poly[0]][1], points[poly[0]][2],
                                points[poly[1]][0], points[poly[1]][1], points[poly[1]][2],
                                points[poly[2]][0], points[poly[2]][1], points[poly[2]][2], c0);
    else {
        float x0=points[poly[0]][0], y0=points[poly[0]][1], z0=points[poly[0]][2];
        for (int i = 0; i < size - 2; ++i) {
            drawTriangle(x0,y0,z0,
                         points[poly[1]][0], points[poly[1]][1], points[poly[1]][2],
                         points[poly[2]][0], points[poly[2]][1], points[poly[2]][2], c0);
            poly = &poly[1];
        }
    }
}


inline bool belongs(int val, int a, int b) {
    return val > 0 || (val == 0 && (a > 0 || (a == 0 && b > 0)));
}

inline float triangle_area (float x1, float y1, float x2, float y2, float x3, float y3) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}


//void QRasterizeZBuffer::drawTriangle(const Vector3D &p0, const Vector3D &p1, const Vector3D &p2, QRColor c) {
void QRasterizeZBuffer::drawTriangle(float p1x, float p1y, float p1z,
                                     float p2x, float p2y, float p2z,
                                     float p3x, float p3y, float p3z, QRColor c) {
    //cout << p1z << ' ' << p2z << ' ' << p3z << '\n';
    int xl = QRound(p1x), yl = QRound(p1y);
    int xr = QRound(p2x), yr = QRound(p2y);
    int xw = QRound(p3x), yw = QRound(p3y);
    xl = max(0, min(xl, w-1)), yl=max(0, min(yl, h-1));
    xr = max(0, min(xr, w-1)), yr=max(0, min(yr, h-1));
    xw = max(0, min(xw, w-1)), yw=max(0, min(yw, h-1));

    float zl = p1z, zr = p2z, zw = p3z;
    float S0 = fabs(triangle_area(p3x, p3y, p1x, p1y, p2x, p2y));

    // line and dot drawing
    if (triangle_area(xw, yw, xl, yl, xr, yr) == 0) {
        if (xl == xr && yl == yr && xl==xw && yl==yw) {
            pixel_lock.lock();
            if (zl - zbuf[yl*w + xl] < -QREPS) {
                img->setPixel(xl, yl, c);
                zbuf[yl*w + xl] = zl;
            }
            pixel_lock.unlock();
            return;
        };

        if (xl == xr && xl == xw) {
            if (yl > yr) swap(xl, xr), swap(yl, yr), swap(zl, zr);
            if (yw < yl) swap(xl, xw), swap(yl, yw), swap(zl, zw);
            else if (yw > yr) swap(xw, xr), swap(yw, yr), swap(zw, zr);
        }
        else {
            if (xl > xr) swap(xl, xr), swap(yl, yr), swap(zl, zr);
            if (xw < xl) swap(xl, xw), swap(yl, yw), swap(zl, zw);
            else if (xw > xr) swap(xw, xr), swap(yw, yr), swap(zw, zr);
        }

        float z = zl, dz;
        if (abs(xr-xl) < abs(yr-yl)) {
            if (yl > yr) swap(xl, xr), swap(yl, yr), swap(zl, zr);
            dz = (zr - zl) / (yr - yl + 0.);
            int pos = yl*w, x = xl;
            float dx = (xr - xl) / (yr - yl + 0.), fx = xl;
            for (int y = yl; y != yr; ++y) {
                pixel_lock.lock();
                if (z - zbuf[pos + x] < -QREPS) {
                    img->setPixel(x, y, c);
                    zbuf[pos + x] = z;
                }
                pixel_lock.unlock();
                pos += w;
                fx += dx;
                z += dz;
                x = QRound(fx);
            }
        }
        else {
            if (xl > xr) swap(xl, xr), swap(yl, yr), swap(zl, zr);
            dz = (zr - zl) / (xr - xl + 0.);
            int pos = yl*w, y = yl;
            float dy = (yr - yl) / (xr - xl + 0.), fy = yl;
            for (int x = xl; x != xr; ++x) {
                pixel_lock.lock();
                if (z - zbuf[pos + x] < -QREPS) {
                    img->setPixel(x, y, c);
                    zbuf[pos + x] = z;
                }
                pixel_lock.unlock();
                fy += dy;
                z += dz;
                y = QRound(fy);
                pos = y*w;
            }
        }
        return;
    }

    float la = p3y - p1y, ra = p3y - p2y, l2a = p2y-p1y;
    float lb = p1x - p3x, rb = p2x - p3x, l2b= p1x-p2x;
    float lc = -la*p3x - lb*p3y, rc = -ra*p3x - rb*p3y, l2c= -l2a*p1x - l2b*p1y;

    float x_test = (p1x + p2x )/2, y_test = (p1y+p2y)/2;
    x_test = (x_test+p3x)/2, y_test= (y_test+p3y)/2;
    if (la*x_test+lb*y_test+lc < -QREPS) la = -la, lb = -lb, lc = -lc;
    if (ra*x_test+rb*y_test+rc < -QREPS) ra = -ra, rb = -rb, rc = -rc;
    if (l2a*x_test+l2b*y_test+l2c < -QREPS) l2a = -l2a, l2b = -l2b, l2c = -l2c;

    int x=x_test, y=min(min(yl,yr), yw);
    int pos = y*w, ymax = max(max(yl,yr),yw);
    if (la < -0) swap(la, ra), swap(lb, rb), swap(lc, rc);
    if (la < -0) swap(la, l2a), swap(lb, l2b), swap(lc, l2c);
    if (ra > 0) swap(ra, l2a), swap(rb, l2b), swap(rc, l2c);
    if (la < 0 || ra > -0) return;

    // 3line = 1: left; -1 - right
    char third_line = fabs(l2a) < QREPS_MINI ? 0 : 1;
    if (third_line && l2a < 0) third_line = -1;

    float lval0, rval0, l2val0;
    int x0;
    float b10, b20, b1, b2;
    float z;

    float lval = la*x + lb*y + lc, rval = ra*x + rb*y + rc, l2val = l2a*x + l2b*y + l2c;
    b1 = triangle_area(p3x, p3y, p1x, p1y, x, y);
    b2 = triangle_area(p3x, p3y, p2x, p2y, x, y);
    float dyl = p3y-p1y, dyr=p3y-p2y, dxl=p1x-p3x, dxr=p2x-p3x;

    for(; y <= ymax; ++y) {
        x0 = x, b10 = b1, b20 = b2;
        lval0=lval, rval0=rval; l2val0=l2val;
        while (rval > -QREPS_MINI && (third_line!=-1 || l2val > -QREPS_MINI)) {
            // todo here may be full-check for borders (left-up rule)
            if (lval > -QREPS_MINI && l2val > -QREPS_MINI) {
                z = (fabs(b1)*zr + fabs(b2)*zl + (S0 - fabs(b1) - fabs(b2))*zw) / S0;
                pixel_lock.lock();
                if (z - zbuf[pos+x] < -QREPS) {
                    img->setPixel(x, y, c);
                    zbuf[pos+x] = z;
                }
                pixel_lock.unlock();
            }
            lval += la, rval += ra; l2val += l2a;
            x++;
            b1 += dyl, b2 += dyr;
        }

        x = x0;
        b1 = b10, b2 = b20;
        lval = lval0, rval=rval0, l2val=l2val0;
        while (lval > -QREPS_MINI && (third_line!=1 || l2val > -QREPS_MINI)) {
            if (rval > -QREPS_MINI && l2val > -QREPS_MINI) {
                z = (fabs(b1)*zr + fabs(b2)*zl + (S0 - fabs(b1) - fabs(b2))*zw) / S0;
                pixel_lock.lock();
                if (z - zbuf[pos+x] <-QREPS) {
                    img->setPixel(x, y, c);
                    zbuf[pos+x] = z;
                }
                pixel_lock.unlock();
            }
            lval -= la, rval -= ra; l2val -= l2a;
            x--;
            b1 -= dyl, b2 -= dyr;
        }
        pos += w;
        lval += lb, rval += rb, l2val += l2b;
        b1 += dxl, b2 += dxr;
    }

}

void QRasterizeZBuffer::clearBuf() {
    int sz = w*h;
    for (int i = 0; i < sz; i+= h)
        memcpy(&zbuf[i], row_example, sizeof(float)*w);

    memset(img->getData(), 0, w*h*4);
}

void QRasterizeZBuffer::clearZBufOnly() {
    int sz = w*h;
    for (int i = 0; i < sz; i+= h)
        memcpy(&zbuf[i], row_example, sizeof(float)*w);
}

bool is_black(uchar *a) {
    return !a[0] && !a[1] && !a[2] && !a[3];
}

void QRasterizeZBuffer::fillMissing() {
    auto data = img->getData();
    int pos=0;
    int border=2;
    for (int k = 0; k < border; ++k)
        for (int j = 0; j < w; ++j) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}

    for (int i = border; i < h-border; ++i) {
        for (int k = 0; k < border; ++k) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}

        for (int j = border; j < w-border; ++j) {
            if (is_black(&data[pos]) && !is_black(&data[pos-4]) && !is_black(&data[pos+4]))
                data[pos] = data[pos - 4], data[pos + 1] = data[pos - 3],
                data[pos + 2] = data[pos - 2], data[pos +3] = data[pos - 1];
            pos += 4;
        }

        for (int k = 0; k < border; ++k) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}
    }

    for (int k = 0; k < border; ++k)
        for (int j = 0; j < w; ++j) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}
}


/*void QRasterizeZBuffer::fillMissing() {
    auto data = img->getData();

    int pos=0;
    int border=2;
    for (int k = 0; k < border; ++k)
        for (int j = 0; j < w; ++j) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}

    int last_active;
    for (int i = border; i < h-border; ++i) {
        for (int k = 0; k < border; ++k) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}
        last_active=-1;
        for (int j = border; j < w-border; ++j, pos+=4) {
            if (!is_black(&data[pos])) {
                if (last_active >= 0) {
                    for (int k = last_active+4; k < pos; k += 4) {
                        data[k] = data[k - 4], data[k + 1] = data[k - 3],
                        data[k + 2] = data[k - 2], data[k + 3] = data[k - 1];
                    }
                    last_active = -1;
                }
            }
            else if (last_active < 0)
                last_active = pos - 4;

        }

        for (int k = 0; k < border; ++k) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}
    }

    for (int k = 0; k < border; ++k)
        for (int j = 0; j < w; ++j) {data[pos++] = 0; data[pos++] = 0; data[pos++] = 0; data[pos++] = 0;}
}*/