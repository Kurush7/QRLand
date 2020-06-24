//
// Created by kurush on 24.06.2020.
//

#include "QRCamera3D.h"
#include "mementos/QRCamera3DMemento.h"

using namespace std;

unique_ptr<QRMemento> QRCamera3D::save() {
    return unique_ptr<QRMemento>(new Camera3DMemento(p));
}

double QRCamera3D::getWidth() const {
    return width;
}

void QRCamera3D::setWidth(double width) {
    QRCamera3D::width = width;
}

double QRCamera3D::getHeight() const {
    return height;
}

void QRCamera3D::setHeight(double height) {
    QRCamera3D::height = height;
}