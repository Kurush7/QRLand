//
// Created by kurush on 01.06.2020.
//

#include "QTPainter.h"

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

void QTPainter::paint() {
    canvas->clearObjects();
    // todo cutting

    for (auto p: points)
        canvas->addObject(p);
    for (auto l: lines)
        canvas->addObject(l);

    canvas->repaint();
}