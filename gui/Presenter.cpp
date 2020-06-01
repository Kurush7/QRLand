#include <cmath>

#include <QtGui>
#include "Presenter.h"
#include "MainWindow.h"


QColor defineColor(QRColor c) {
    QColor st;
    st.setRgb(c.r,c.g,c.b);
    return st;
}
QRColor defineColor(QColor c) {
    QRColor st;
    c.getRgb(&st.r, &st.g, &st.b);
    return st;
}



Presenter::Presenter(MainWindow &w): window(w) {
}

void Presenter::backChangeColor() {
    window.canvas->setBgColor(window.backColorEdit->getColor());
    window.canvas->repaint();
}

void Presenter::setPainter() {
    painter = std::shared_ptr<Painter>(new QTPainter(window.canvas));
}

void Presenter::loadModel() {
    auto filename = window.fileName->text().toUtf8().constData();
    facade.addModel(filename);
    facade.draw(painter);
}
void Presenter::openFile() {
    auto fileName = QFileDialog::getOpenFileName(&window, tr("Open File"), "~", tr("*"));
    window.fileName->setText(fileName);
    QFile file(fileName);

    if (!file.exists()) {
        window.fileName->setText("файл не найден!");
    }
}

void Presenter::select(double x, double y) {
    facade.select(x, y);
    facade.draw(painter);
    window.visibilityManager->setActive("selection", !facade.isEmptySelection());

}

void Presenter::selChangeColor() {
    QColor pColor = window.selPointColorEdit->getColor();
    QColor eColor = window.selEdgeColorEdit->getColor();
    facade.setColor(defineColor(pColor), defineColor(eColor));
    facade.draw(painter);
}
void Presenter::selDelete() {
    facade.deleteSelection();
    facade.draw(painter);
}

void Presenter::transform(TransformStateDir d) {
    double x, y, z;
    defineTransformParams(x,y,z, d);

    if (window.moveRad->isChecked())
        facade.move(x,y,z);
    else if (window.rotateRad->isChecked())
        facade.rotate(x,y,z);
    else {
        facade.scale(x,y,z);
    }
    facade.draw(painter);
}


void Presenter::defineTransformParams(double &x, double &y, double &z, TransformStateDir d) {
    double val = 0;
    if (window.moveRad->isChecked())
        val = MOVE_UNIT_VAL;
    else if (window.rotateRad->isChecked())
        val = ROTATE_UNIT_VAL;
    else {
        val = SCALE_UNIT_VAL;
    }

    if (d == Xdec || d == Ydec || d == Zdec) {
        if (window.scaleRad->isChecked())
            val = 1 / val;
        else
            val = -val;
    }

    x = 0, y = 0, z = 0;
    if (window.scaleRad->isChecked()) {
        x = y = z = 1;
    }

    if (d == Xdec || d == Xinc)
        x = val;
    else if (d == Ydec || d == Yinc)
        y = val;
    else
        z = val;
}

void Presenter::undo() {
    facade.undo();
    facade.draw(painter);

}