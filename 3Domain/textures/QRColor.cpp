//
// Created by kurush on 01.07.2020.
//

#include "QRColor.h"

std::ostream& operator<<(std::ostream &os, const QRColor &c) {
    os << "<QRColor: r=" << (int)c.r << " g=" << (int)c.g << " b=" << (int)c.b << ">";
    return os;
}

QRColor mixColors(const QRColor &a, const QRColor &b) {
    return QRColor((a.r+b.r)/2, (a.g+b.g)/2, (a.b+b.b)/2, (a.alpha+b.alpha)/2);
}