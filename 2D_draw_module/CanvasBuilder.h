//
// Created by kurush on 15.04.2020.
//

#ifndef KG_CANVASBUILDER_H
#define KG_CANVASBUILDER_H

#include "PolyBuilder.h"
#include "figures/Figures.h"

enum buildState {
    LINE,
    RECTANGLE,
    POLYGON
};

class CanvasBuilder: public PolyBuilder {
    Q_OBJECT
public:
    explicit CanvasBuilder(int w, int h, QWidget *parent = nullptr,
                         QColor fill = Qt::white): PolyBuilder(w, h, parent, fill) {}

    virtual void enablePanel(bool enablePolygon=true, bool enableRect=true);
    void keyPressEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent* event);
    void sendFigure();

signals:
    void sendLine(Line*);
    void sendRect(Rectangle*);
    void sendPolygon(Polygon*);
    void cleared();
private:
    buildState state;
};


#endif //KG_CANVASBUILDER_H
