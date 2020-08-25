#include <cmath>

#include <QtGui>
#include <QMessageBox>

#include "Presenter.h"
#include "MainWindow.h"

using namespace std;
using namespace chrono;

Presenter::Presenter(MainWindow &w): window(w) {
    image = sptr<QRImage>(new ImageQT(window.canvas));
    hmap_image = sptr<QRImage>(new ImageQT(window.hmap));
    facade = sptr<Facade>(new Facade(image, hmap_image));

    //facade->addCube(10,0,0,0, QRColor("red"));
    draw();
}

void Presenter::transform(QRKey d) {
    float x, y, z;
    defineTransformParams(x,y,z, d);

    if (window.moveRad->isChecked())
        facade->moveCamera(x,y,z);
    else if (window.rotateRad->isChecked())
        facade->rotateCamera(x,y,z);
    else {
        facade->scaleCamera(x,y,z);
    }
    draw(window.scaleRad->isChecked());
}

void Presenter::scale(float times) {
    float x = 1 + SCALE_FULL_ROTATE_VAL * times;
    float y = 1 + SCALE_FULL_ROTATE_VAL * times;
    float z = 1 + SCALE_FULL_ROTATE_VAL * times;

    facade->scaleCamera(x,y,z);
    draw(window.scaleRad->isChecked());
}


void Presenter::defineTransformParams(float &x, float &y, float &z, QRKey d) {
    float val = 0;
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
    draw();
}

void Presenter::setWaterVisible() {
    facade->setWaterVisible(window.waterCheckBox->isChecked());
}

void Presenter::erosionIteration() {
    facade->erosionIteration();
}

void Presenter::changeCamera() {
    if (is1Active && window.view1Rad->isChecked()) return;
    else if (!is1Active && window.view2Rad->isChecked()) return;

    if (window.view1Rad->isChecked()) is1Active = true;
    else is1Active = false;

    facade->changeCamera();
}


void Presenter::draw(bool reset) {
    static int cnt = 0;
    if (reset) draw_cnt = 0, draw_time_msec = 0;

    system_clock::time_point start = system_clock::now();
    facade->draw();
    system_clock::time_point end = system_clock::now();
    float time = (end - start).count();    // nanosecs
    time /= 1e6;  // milisecs
    if (draw_cnt > 50) draw_cnt = 0, draw_time_msec=0;
    draw_time_msec = (draw_time_msec * draw_cnt + time) / (draw_cnt+1);
    draw_cnt++;
    window.drawTimeLabel->setText("среднее ремя отрисовки: " + QString::number(draw_time_msec) +
    " msec ... (" + QString::number(int(1000/draw_time_msec) )+ " FPS)");
}