//
// Created by kurush on 13.02.2020.
//

#ifndef KG_GRAPHICFIELD_H
#define KG_GRAPHICFIELD_H


#include "QRCanvasViewer.h"
#include "QRActionManager.h"

class QRCanvas : public QRActionManager {
Q_OBJECT
public:
    explicit QRCanvas(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white);
    virtual ~QRCanvas() {delete pixels;}

    uchar* getData() {return pixels;}

    int getWidth() {return width;}
    int getHeight() {return height;}

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

protected:
    int width, height;
    uchar *pixels;
};

#endif //KG_GRAPHICFIELD_H