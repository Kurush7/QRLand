//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

void QRasterizeZBuffer::draw(RenderPolygon &poly, const sptr<QRTexture> &txt, const Vector3D &norm) {
    normal = norm;
    texture = txt;
    if (isRightRotate(poly[0], poly[1], poly[2]))   // needed right, but y = -y, so...
        poly.reverse();

    for (auto p: poly)
        cout << p << ' ' ;
    cout << '\n';

    int left = 0, right = -1, ll, rr;   // ll - after left, rr - after right
    int n = poly.getSize();
    for (int i = 0; i < n; ++i) {
        poly[i][0] = QRound(poly[i][0]);
        poly[i][1] = QRound(poly[i][1]);
        if (poly[i][1] < poly[left][1])
            left = right = i;
        else if (poly[i][1] == poly[left][1]) {
            if (poly[i][0] < poly[left][0]) right = left, left = i;
            else right = i;
        }
    }
    for (auto p: poly)
        cout << p << ' ' ;
    cout << '\n';

    if (right == -1) right = left;
    ll = (left-1 + n) % n;
    rr = (right+1) % n;

    double xl = poly[left][0] , xr = poly[right][0];
    int y = poly[left][1];
    double bl, br;
    // horizontals are impossible
    bl = (poly[ll][0] - poly[left][0]+0.) / (poly[left][1] - poly[ll][1]+0.);
    br = (poly[rr][0] - poly[right][0]+0.) / (poly[right][1] - poly[rr][1]+0.);
    cout << "**********ANGLES*******:  " << bl << ' ' << br << '\n';

    double zl, zr, dzl, dzr;
    zl = poly[left][2];
    dzl = (poly[ll][2] - zl) / (poly[left][1] - poly[ll][1]+0.);
    zr = poly[right][2];
    dzr = (poly[rr][2] - zr) / (poly[right][1] - poly[rr][1]+0.);

    // from min y to max. axis: (0,0) is up-left
    while(true) {
        fillRow(QRound(xl), QRound(xr), y, zl, zr);
        y++;
        if (y > poly[ll][1]) {
            if (y > poly[rr][1]) {
                if (rr == ll || (ll == right && left == rr))
                    break;
                right = rr;
                rr = (right+1 + n) % n;
                br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1] + 0.);
                zr = poly[right][2];
                dzr = (poly[rr][2] - zr) / (poly[right][1] - poly[rr][1]+0.);
                cout << "**********ANGLES*******:  " << bl << ' ' << br << '\n';
            }
            left = ll;
            ll = (left-1 + n) % n;
            bl = (poly[ll][0] - poly[left][0]) / (poly[left][1] - poly[ll][1] + 0.);
            zl = poly[left][2];
            dzl = (poly[ll][2] - zl) / (poly[left][1] - poly[ll][1]+0.);
            cout << "**********ANGLES*******:  " << bl << ' ' << br << '\n';
        }
        else if (y > poly[rr][1]) {
            right = rr;
            rr = (right+1 + n) % n;
            br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1] + 0.);
            zr = poly[right][2];
            dzr = (poly[rr][2] - zr) / (poly[right][1] - poly[rr][1]+0.);
            cout << "**********ANGLES*******:  " << bl << ' ' << br << '\n';
        }
        xl = xl - bl;   // ax+by+c=0 -> x=-c/a +y*(-b/a)
        xr = xr - br;
        zl += dzl;
        zr += dzr;
    }
}

void QRasterizeZBuffer::fillRow(int xl, int xr, int y, int zl ,int zr) {
    xl = max(0, xl);
    xr = min(img->getWidth()-1, xr);
    if (xl == xr) return;
    double dz = (zr - zl + 0.) / (xr - xl + 0.);
    double z = zl;
    cout << "fill: x[" << xl << ' ' << xr << "] y: " << y << '\n';
    // todo add lights here
    // todo manage z-buffering
    for (int x = xl; x <= xr; ++x) {
        if (z < zbuf[x][y]) {
            Vector3D i = {0,0,0,0};
            for (auto li = lights; li; ++li)
                i += (*li).get()->getIntensity({0, 0, 0}, normal); // todo no pos here
            QRColor c = texture->getColor();
            //cout <<(int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << ":: ";
            c.r = i[0] * c.r;
            c.g = i[1] * c.g;
            c.b = i[2] * c.b;
            //todo cout << c << '\n';
            //cout << i << " => " << (int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << '\n';
            img->setPixel(x, y, c);
            zbuf[x][y] = z;
            z += dz;
        }
    }
}

void QRasterizeZBuffer::clearBuf() {
    auto c = QRColor("black");
    for (int i = 0; i < h; ++i) {
        zbuf[i] = (double *) malloc(sizeof(double) * w);
        for (int j = 0; j < w; ++j) {
            zbuf[i][j] = QRINF;
            img->setPixel(i, j, c);
        }
    }
}