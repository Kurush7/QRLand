#include <cmath>

#include <QtGui>
#include <QMessageBox>
#include "Presenter.h"
#include "MainWindow.h"

using namespace std;

Presenter::Presenter(MainWindow &w): window(w) {
    image = sptr<QRImage>(new ImageQT(window.canvas));
    facade = sptr<Facade>(new Facade(image));

    facade->addCube(10,0,0,0, QRColor("red"));
    facade->draw();
}

void Presenter::backChangeColor() {
    //window.canvas->setBgColor(window.backColorEdit->getColor());
    //window.canvas->repaint();
}

void Presenter::transform(QRKey d) {
    double x, y, z;
    defineTransformParams(x,y,z, d);

    if (window.moveRad->isChecked())
        facade->moveCamera(x,y,z);
    else if (window.rotateRad->isChecked())
        facade->rotateCamera(x,y,z);
    else {
        facade->scaleCamera(x,y,z);
    }
    facade->draw();
}


void Presenter::defineTransformParams(double &x, double &y, double &z, QRKey d) {
    double val = 0;
    if (window.moveRad->isChecked())
        val = MOVE_UNIT_VAL;
    else if (window.rotateRad->isChecked())
        val = ROTATE_UNIT_VAL;
    else {
        val = SCALE_UNIT_VAL;
    }

    // w instead of s, for y-axis is turned.... todo fuck
    if (d == QRKey_A || d == QRKey_W || d == QRKey_E) {
        if (window.scaleRad->isChecked())
            val = 1 / val;
        else
            val = -val;
    }

    x = 0, y = 0, z = 0;
    if (window.scaleRad->isChecked()) {
        x = y = z = 1;
    }

    if (d == QRKey_A || d == QRKey_D)
        x = val;
    else if (d == QRKey_W || d == QRKey_S)
        y = val;
    else
        z = val;
}

void Presenter::undo() {
    facade->undo();
    facade->draw();
}

void Presenter::addCube(double a) {
    double x = window.xDropEdit->text().toDouble();
    double y = window.xDropEdit->text().toDouble();
    double z = window.xDropEdit->text().toDouble();
    QColor c = window.figureColorEdit->getColor();
    QRColor qrc = defineColor(c);
    facade->addCube(a, x, y, z, qrc);
    facade->draw();
}