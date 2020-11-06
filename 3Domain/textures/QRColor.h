//
// Created by kurush on 25.06.2020.
//

#ifndef BIG3DFLUFFY_QRCOLOR_H
#define BIG3DFLUFFY_QRCOLOR_H

#include <map>
#include <string>
#include <iostream>

#include "../QRDefines.h"

// todo move to textures directory/file
class QRColor {
public:
    // todo accurate color string mapping: base class with map and 'addName' method
    QRColor() {}
    QRColor (std::string name) {
        if (name == "white") r = b = g = 255;
        if (name == "black") r=b=g=0;
        if (name == "green") g = 255;
        if (name == "blue") b = 255;
        if (name == "red") r = 255;
    }
    QRColor(int r, int g, int b, int alpha=255): r(r), g(g), b(b), alpha(alpha) {}
    bool operator==(const QRColor &c) const{return r==c.r && b==c.b && g==c.g;}
    u_char r=0, g=0, b=0, alpha=255;
};

QRColor mixColors(const QRColor &a, const QRColor &b);

// used for point & edge drawing. legacy code
class Style {
public:
    Style() {}
    Style(QRColor c): color(c) {}
    QRColor color = QRColor("white");

    virtual bool operator==(const Style &s) const{return color == s.color;}
};
class QRPointStyle: public Style {
public:
    QRPointStyle(){}
    QRPointStyle(QRColor c): Style(c) {}

};
class QREdgeStyle: public Style {
public:
    QREdgeStyle(){}
    QREdgeStyle(QRColor c): Style(c) {}
};

class ColorKeeper {
public:
    ColorKeeper(QRPointStyle p, QREdgeStyle e): pointStyle(p), edgeStyle(e) {}
    QRPointStyle pointStyle;
    QREdgeStyle edgeStyle;
};


std::ostream& operator<<(std::ostream &os, const QRColor &c);

#endif //BIG3DFLUFFY_QRCOLOR_H
