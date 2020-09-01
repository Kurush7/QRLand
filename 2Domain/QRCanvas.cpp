//
// Created by kurush on 13.02.2020.
//

#include "QRCanvas.h"

using namespace std;


QRCanvas::QRCanvas(int w, int h, QWidget *parent)
        : QRActionManager(parent), width(w), height(h) {
    setFixedSize(w,h);
    repaint();

    pixels = new uchar[width*height*4];
    //for (int i = 0; i < width*height; i+=4)
        //pixels[i] = fill.red(), pixels[i+1] = fill.green(),
        //pixels[i+2] = fill.blue(), pixels[i+3] = fill.alpha();

    repaint();
}

void QRCanvas::initializeGL() {/*todo*/}

void QRCanvas::resizeGL(int width, int height) {/*todo*/}

void QRCanvas::paintGL() {
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

