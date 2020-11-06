//
// Created by kurush on 25.06.2020.
//

#ifndef BIG3DFLUFFY_QRTEXTURE_H
#define BIG3DFLUFFY_QRTEXTURE_H

#include "QRColor.h"

class QRTexture {
public:
    // todo color by coordinate, method 'is singleColor'
    virtual QRColor getColor() const = 0;
    virtual void setColor(const QRColor &c) = 0; // todo well... not good
};

class ColorTexture: public QRTexture {
public:
    ColorTexture(const QRColor &c): color(c) {}
    ColorTexture(std::string name): color(name) {}
    ColorTexture(int r, int g, int b, int alpha=255): color(r,g,b,alpha) {}
    virtual QRColor getColor() const {return color;}
    virtual void setColor(const QRColor &c) {color = c;}

    QRColor color;
};

#endif //BIG3DFLUFFY_QRTEXTURE_H
