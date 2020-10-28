#include <cmath>

#include <QtGui>
#include <QMessageBox>

#include "EditorPresenter.h"
#include "EditorWindow.h"

using namespace std;
using namespace chrono;

EditorPresenter::EditorPresenter(EditorWindow &w, ModelInitData data,  bool initFacade): window(w) {
    image = sptr<QRImage>(new ImageQT(window.canvas));
    hmap_image = sptr<QRImage>(new ImageQT(window.canvasMini));
    if (initFacade)
        facade = sptr<Facade>(new Facade(data, image, hmap_image));
    else
        facade = sptr<Facade>(new Facade(image, hmap_image));
}

void EditorPresenter::transform(QRKey d) {
    float x, y, z;
    defineTransformParams(x,y,z, d);

    facade->moveCamera(x,y,z);

    draw();
}

void EditorPresenter::transformMouse(float dx, float dy) {
    if (fabs(dx) >= fabs(dy)) dy = 0;
    else dx = 0;
    float x=0, y=0, z=0;
    float k = 500;
    z = dx / k;
    x = dy * cos(rotCamera) / k;
    y = dy * -sin(rotCamera) / k;

    facade->rotateCamera(x,y,z);
    rotCamera += z;
    draw();
}

void EditorPresenter::scale(float times) {
    float x = 1 + SCALE_FULL_ROTATE_VAL * times;
    float y = 1 + SCALE_FULL_ROTATE_VAL * times;
    float z = 1 + SCALE_FULL_ROTATE_VAL * times;

    facade->scaleCamera(x,y,z);
    draw();
}


void EditorPresenter::defineTransformParams(float &x, float &y, float &z, QRKey d) {
    float val = 0;
        val = MOVE_UNIT_VAL;

    if (d == QRKey_A || d == QRKey_W || d == QRKey_E) {
            val = -val;
    }

    x = 0, y = 0, z = 0;
    if (d == QRKey_A || d == QRKey_D)
        x = val;
    else if (d == QRKey_W || d == QRKey_S)
        y = val;
    else
        z = val;
}

void EditorPresenter::undo() {
    facade->undo();
    draw();
}

void EditorPresenter::setWaterVisible() {} //todo delete
void EditorPresenter::setShadesVisible() {} //todo delete

void EditorPresenter::erosionIteration() {
    facade->erosionIteration();
}

void EditorPresenter::changeCamera() {} // todo delete

void EditorPresenter::scaleGrid() {
    facade->scaleGrid();
}

void EditorPresenter::process() {
    facade->process();
}

void EditorPresenter::draw(bool reset) {
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
    int w = facade->builder->getHeightMap().width();
    int h = facade->builder->getHeightMap().height();
    window.drawTimeLabel->setText("размер модели:" +QString::number(w)+ "*" +QString::number(w)+
    "\nсреднее время отрисовки: " + QString::number(draw_time_msec) +
    " msec ... (" + QString::number(int(1000/draw_time_msec) )+ " FPS)");
}

void EditorPresenter::updateMiniCoords(float x0, float y0) {
    auto p = window.canvasMini->mapFromGlobal(QCursor::pos());
    int x = (p.x() + 0.) / window.canvasMini->getWidth() * (facade->builder->getHeightMap().width() + 0.);
    int y = (p.y() + 0.) / window.canvasMini->getHeight() * (facade->builder->getHeightMap().height() + 0.);
    float z = facade->builder->getHeightMap()[y][x];
    float xf = round(x * facade->builder->getWorldStep() * 100.) / 100.;
    float yf = round(y * facade->builder->getWorldStep() * 100.) / 100.;
    z = round(z * 100.) / 100.;
    window.miniScreenX->setText("x: " + QString::number(xf));
    window.miniScreenY->setText("y: " + QString::number(yf));
    window.miniScreenZ->setText("z: " + QString::number(z));
}