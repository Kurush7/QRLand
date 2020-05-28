//
// Created by kurush on 04.03.2020.
//

#include "LineSpacer.h"

LineSpacer::LineSpacer(QString style, int width) {
    auto lay = new QHBoxLayout();
    setLayout(lay);
    setFrameShape(QFrame::Box);
    setFixedHeight(2);
    if (width > 0)
        setFixedWidth(width);
    setStyleSheet(style);
    setBackgroundRole(QPalette::AlternateBase);
}