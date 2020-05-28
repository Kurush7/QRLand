//
// Created by kurush on 15.02.2020.
//

#ifndef KG_DRAWSTYLES_H
#define KG_DRAWSTYLES_H


#include <QtCore/Qt>
#include <QColor>

struct PointStyle
{
    QColor color;
    int size;

    PointStyle(Qt::GlobalColor c, int s): color(c), size(s) {}
    PointStyle(QColor c, int s): color(c), size(s) {}
    PointStyle(): color(Qt::black), size(5) {}
};

struct LineStyle
{
    QColor color;
    int width;
    Qt::PenStyle style;

    LineStyle(QColor c, int w, Qt::PenStyle s): color(c), width(w), style(s) {}
    LineStyle(): color(Qt::black), width(1), style(Qt::SolidLine) {}
};

#endif //KG_DRAWSTYLES_H
