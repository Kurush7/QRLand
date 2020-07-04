//
// Created by kurush on 13.02.2020.
//

#include "QRCanvas.h"

using namespace std;

QRCanvas::QRCanvas(int w, int h, QWidget *parent, QColor fill)
        : QRActionManager(parent), width(w), height(h) {
    //QPalette p(palette());
    //p.setColor(QPalette::Background, QColor(255,255,255,255));
    //setAutoFillBackground(true);
    //setPalette(p);
    imageLabel = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    setFixedSize(w, h);
    bgColor = fill;

    img = sptr<QImage>(new QImage(w, h, QImage::Format_ARGB32));
    zoomed_img = sptr<QImage>(new QImage(w, h, QImage::Format_ARGB32));
    viewer = QRCanvasViewer(img);

    refillBg();
    repaint();
}

void QRCanvas::refillBg() {
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            img->setPixelColor(i, j, bgColor);
    repaint();
}

void QRCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painterMain(this);
    viewer.drawOn(zoomed_img);  // copy from img to zoomed_img with zooming
    painterMain.drawImage(QRect(0,0, width, height), *zoomed_img);
}
