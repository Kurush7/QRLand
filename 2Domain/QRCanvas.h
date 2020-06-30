//
// Created by kurush on 13.02.2020.
//

#ifndef KG_GRAPHICFIELD_H
#define KG_GRAPHICFIELD_H

#include <QPainter>

#include "QRCanvasViewer.h"
#include "QRActionManager.h"

class QRCanvas : public QRActionManager {
Q_OBJECT
public:
    explicit QRCanvas(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white);
    virtual ~QRCanvas() = default;

    virtual void paintEvent(QPaintEvent *event);

    void setBgColor(QColor c) {bgColor = c;}
    void refillBg();

    sptr<QImage> getImage() {return img;}

    int getWidth() {return width;}
    int getHeight() {return height;}

protected:
    int width, height;
    QColor bgColor;
    QRCanvasViewer viewer;
    sptr<QImage> img, zoomed_img;
};

#endif //KG_GRAPHICFIELD_H