//
// Created by kurush on 13.02.2020.
//

#include "QRCanvas.h"

using namespace std;


QRCanvas::QRCanvas(int w, int h, QWidget *parent, QColor fill)
        : QRActionManager(parent), width(w), height(h) {
    setFixedSize(w,h);


    pixels = new uchar[width*height*4];
    for (int i = 0; i < width*height*4; ++i)
        pixels[i] = 127;

    repaint();
}

void QRCanvas::initializeGL() {
    //glClearColor(0.0, 0.0, 0.0, 1.0);

    //glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_TEXTURE_2D);
}

void QRCanvas::resizeGL(int width, int height) {
    // todo
}

void QRCanvas::paintGL() {
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

