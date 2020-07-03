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
    ImageQT(const sptr<QRCanvas> &canvas): canvas(canvas) {
        image = canvas->getImage().get();
    }
    virtual void setPixel(int x, int y, const QRColor &color) {
        // todo too long
        image->setPixelColor(x, y, QColor(color.r, color.g, color.b));
    }
    virtual const QRColor getPixel(int x, int y) const {
        return defineColor(image->pixelColor(x, y));
    }

    virtual void repaint() {canvas->repaint();}
    virtual void refillBg() {canvas->refillBg();}

    virtual int getWidth() {return canvas->getWidth();}
    virtual int getHeight() {return canvas->getHeight();}

private:
    sptr<QRCanvas> canvas;
    QImage *image;
};


// todo creator

#endif //BIG3DFLUFFY_QRIMAGEQT_H
