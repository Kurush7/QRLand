//
// Created by kurush on 01.07.2020.
//

#include "QRColor.h"

std::ostream& operator<<(std::ostream &os, const QRColor &c) {
    os << "<QRColor: r=" << (int)c.r << " g=" << (int)c.g << " b=" << (int)c.b << ">";
    return os;
}