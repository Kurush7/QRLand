//
// Created by kurush on 28.06.2020.
//

#include "QRCanvasViewer.h"

using namespace std;

void QRCanvasViewer::validateD() {
    dx = max(0., dx);
    dy = max(0., dy);
    dx = min(dx, src->width() * (1 - 1/k));
    dy = min(dy, src->height() * (1 - 1/k));
}
void QRCanvasViewer::validateK() {
    k = max(1., k);
}

void QRCanvasViewer::zoom(Point center, double k_new) {
    double dx_new = (center.x - src->width()/2) / k / zoomSensivity;
    double dy_new = (center.y - src->height()/2) / k / zoomSensivity;
    k *= k_new;
    validateK();

    dx += dx_new;
    dy += dy_new;
    validateD();
}

void QRCanvasViewer::move(double _dx, double _dy) {
    dx += _dx / moveSensivity;
    dy += _dy / moveSensivity;
    validateD();
}

void QRCanvasViewer::reset() {
    dx = dy = 0;
    k = 1;
}

void QRCanvasViewer::drawOn(sptr<QImage> img) {
    for (int i = 0; i < img->height(); ++i)
        for (int j = 0; j < img->width(); ++j) {
            img->setPixel(j, i, src->pixel(dx + j/k, dy + i/k));
        }
}
