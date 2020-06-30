//
// Created by kurush on 30.06.2020.
//

#include "QRasterizeZBuffer.h"

void QRasterizeZBuffer::draw(RenderPolygon &poly, const sptr<QRTexture> &texture) {
    if (!isRightRotate(poly[0], poly[1], poly[2]))
        poly.reverse();

    int left = 0, right = -1, ll, rr;   // ll - after left, rr - after right
    int n = poly.getSize();
    for (int i = 0; i < n; ++i) {
        poly[i][0] = QRound(poly[i][0]);
        poly[i][1] = QRound(poly[i][1]);
        if (poly[i][1] < poly[left][1]) left = i;
        else if (poly[i][1] == poly[left][1] && poly[i][0] < poly[left][0])
            right = left, left = i;
    }
    if (right == -1) right = left;
    ll = (left-1 + n) % n;
    rr = (right+1) % n;

    int xl = poly[left][0] , xr = poly[right][0], y = poly[left][1];
    double bl, br;
    // horizontals are impossible
    bl = (poly[ll][0] - poly[left][0]) / (poly[left][1] - poly[ll][1]);
    br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1]);

    // from min y to max. axis: (0,0) is up-left
    while(true) {
        fillRow(xl, xr, y);
        xl = QRound(xl - bl);   // ax+by+c=0 -> x=-c/a +y*(-b/a)
        xr = QRound(xl - br);
        y++;
        if (y > poly[ll][1]) {
            if (y > poly[rr][1]) {
                if (rr == ll || (ll == right && left == rr))
                    break;
                right = rr;
                rr = (right+1 + n) % n;
                br = (poly[rr][0] - poly[right][0]) / (poly[right][1] - poly[rr][1]);
            }
            left = ll;
            ll = (left-1 + n) % n;
            bl = (poly[ll][0] - poly[left][0]) / (poly[left][1] - poly[ll][1]);
        }
    }
}

void QRasterizeZBuffer::fillRow(int xl, int xr, int y) {

}