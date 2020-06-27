//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRPAINTER_H
#define BIG3DFLUFFY_QRPAINTER_H

#include "objects/QRObject.h"

// todo draw_poly
/* interface for drawers used in 3Domain.
 */
class QRPainter {
public:
    virtual void drawPoint(double x, double y, QRPointStyle) = 0;
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle) = 0;
    virtual void setFrame(double x_center, double y_center, double w, double h) = 0;
    virtual void paint() = 0;   // methods above are supposed to put figs in some buffer, while this really draws
    virtual void reset() = 0;

    virtual QRPointStyle getSelectionPointStyle() = 0;
    virtual QREdgeStyle getSelectionEdgeStyle() = 0;
};

class BasePainterCreator {
public:
    virtual sptr<QRPainter> getPainter() {
        if (!painter)
            painter = create();
        return painter;
    }
protected:
    virtual sptr<QRPainter> create() = 0;
    sptr<QRPainter> painter = nullptr;
};


#endif //BIG3DFLUFFY_QRPAINTER_H
