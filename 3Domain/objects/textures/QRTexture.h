//
// Created by kurush on 25.06.2020.
//

#ifndef BIG3DFLUFFY_QRTEXTURE_H
#define BIG3DFLUFFY_QRTEXTURE_H

#include "QRColor.h"

class QRTexture {
public:
    virtual QRColor getColor() const = 0;
};

class ColorTexture: public QRTexture {
public:
    ColorTexture(const QRColor &c): color(c) {}
    ColorTexture(std::string name): color(name) {}
    ColorTexture(int r, int g, int b, int alpha=255): color(r,g,b,alpha) {}
    virtual QRColor getColor() const {return color;}

    QRColor color;
};

#endif //BIG3DFLUFFY_QRTEXTURE_H
