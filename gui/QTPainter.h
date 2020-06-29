//
// Created by kurush on 01.06.2020.
//

#ifndef BIG3DFLUFFY_QTPAINTER_H
#define BIG3DFLUFFY_QTPAINTER_H

#endif //BIG3DFLUFFY_QTPAINTER_H

#include "../3Domain/3Domain.h"
#include <QColor>
#include "../2DomainOld/Canvas3DViewer.h"


QColor defineColor(QRColor c);
QRColor defineColor(QColor c);

class QTPainter: public QRImage {
public:
    QTPainter(Canvas3DViewer *c): canvas(c) {}
    virtual void drawPoint(double x, double y, QRPointStyle s) {
        points.push_back(new Point(x, y, defineColor(s.color)));
    }
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle s) {
        lines.push_back(new Line(x1, y1, x2, y2, defineColor(s.color)));
    }

    virtual void setFrame(double x_center, double y_center, double w, double h) {
        canvas->setFromBorders(Borders(x_center-w/2, x_center+w/2, y_center-h/2,  y_center+h/2));
    }

    virtual void paint();
    virtual void reset() {
        points.clear();
        lines.clear();
    }

    virtual QRPointStyle getSelectionPointStyle() {return QRPointStyle(QRColor("white"));}
    virtual QREdgeStyle getSelectionEdgeStyle() {return QREdgeStyle(QRColor("red"));}

private:
    Canvas3DViewer *canvas;
    vector<Line*> lines;
    vector<Point*> points;
};



class QTPainterCreator: public BasePainterCreator {
public:
    explicit QTPainterCreator(Canvas3DViewer *canvas): canvas(canvas) {}
protected:
    virtual sptr<QRImage> getPainter() override {
        if (!painter)
            painter = create();
        else
            painter->reset();
        return painter;
    }
    virtual sptr<QRImage> create()  {
        return sptr<QRImage>(new QTPainter(canvas));
    }
private:
    Canvas3DViewer *canvas;
};
