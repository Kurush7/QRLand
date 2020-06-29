//
// Created by kurush on 13.02.2020.
//

#ifndef KG_GRAPHICFIELD_H
#define KG_GRAPHICFIELD_H

#include "QRCanvasViewer.h"


class QRCanvas : public QWidget {
Q_OBJECT
public:
    explicit QRCanvas(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white);
    virtual ~QRCanvas() = default;

    virtual void paintEvent(QPaintEvent *event);

    void setBgColor(QColor c) {bgColor = c;}
    void refillBg();

    sptr<QImage> getImage() {return img;}

protected:
    int width, height;
    QColor bgColor;
    QRCanvasViewer viewer;
    sptr<QImage> img, zoomed_img;
};

#endif //KG_GRAPHICFIELD_H