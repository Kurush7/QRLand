//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRIMAGEQT_H
#define BIG3DFLUFFY_QRIMAGEQT_H

#include "QRImage.h"
#include "QRCanvas.h"

// todo too long with these
QColor defineColor(QRColor c);
QRColor defineColor(QColor c);

class ImageQT: public QRImage {
public:
    ImageQT(const sptr<QRCanvas> &canvas): canvas(canvas) {}
    virtual void setPixel(int x, int y, const QRColor &color) {
        // todo too long
        canvas->getImage()->setPixelColor(x, y, defineColor(color);)
    }
    virtual void repaint() {canvas->repaint();}
    virtual void refillBg() {canvas->refillBg();}

private:
    sptr<QRCanvas> canvas;
};


// todo creator

#endif //BIG3DFLUFFY_QRIMAGEQT_H
