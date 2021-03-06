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
    ~ImageQT() {

    }

    ImageQT(const sptr<QRCanvas> &canvas, const QRColor &bg = QRColor("black"))
    : canvas(canvas), bg(bg) {
        data = canvas->getData();
        w = canvas->getWidth();
        h = canvas->getHeight();
    }
    virtual void setPixel(int x, int y, const QRColor &color) {
        // todo too long, checks for out-of-range?
        int ind = (y*w + x)*4;
        data[ind++] = color.r;
        data[ind++] = color.g;
        data[ind++] = color.b;
        data[ind] = color.alpha;
    }
    virtual const QRColor getPixel(int x, int y) const {
        QRColor color;
        int ind = y*w + x;
        color.r = data[ind++];
        color.g = data[ind++];
        color.b = data[ind++];
        color.alpha = data[ind];
        return color;
    }

    virtual void mixPixel(int x, int y, const QRColor &color) {
        int ind = (y*w + x)*4;
        data[ind] = (data[ind] + color.r)/2; ind++;
        data[ind] = (data[ind] + color.g)/2; ind++;
        data[ind] = (data[ind] + color.b)/2; ind++;
        data[ind] = (data[ind] + color.alpha)/2;;
    }

    virtual void repaint() {canvas->repaint();}

    virtual void setBg(const QRColor &color) {bg = color;}
    virtual void refillBg();

    virtual int getWidth() const {return canvas->getWidth();}
    virtual int getHeight() const {return canvas->getHeight();}
    virtual unsigned char* getData() {return data;}

private:
    sptr<QRCanvas> canvas;
    uchar *data;
    int w, h;

    QRColor bg;
};


// todo creator

#endif //BIG3DFLUFFY_QRIMAGEQT_H
