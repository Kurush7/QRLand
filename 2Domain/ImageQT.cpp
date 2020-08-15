//
// Created by kurush on 28.06.2020.
//

#include "ImageQT.h"

QColor defineColor(QRColor c) {
    QColor st;
    st.setRgb(c.r,c.g,c.b);
    return st;
}
QRColor defineColor(QColor c) {
    QRColor st;
    int a,b,d;
    c.getRgb(&a, &b, &d);
    st.r = a;
    st.g = b;
    st.b = d;
    return st;
}
