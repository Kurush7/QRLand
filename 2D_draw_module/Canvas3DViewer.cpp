//
// Created by kurush on 23.05.2020.
//

#include "Canvas3DViewer.h"

void Canvas3DViewer::keyPressEvent(QKeyEvent *event) {
    if (timer.isActive() || event->isAutoRepeat())
        return;

    if (event->key() == Qt::Key_A) {
        state = Xdec;
    } else if (event->key() == Qt::Key_D) {
        state = Xinc;
    } else if (event->key() == Qt::Key_W) {
        state = Yinc;
    } else if (event->key() == Qt::Key_S) {
        state = Ydec;
    } else if (event->key() == Qt::Key_R) {
        state = Zinc;
    } else if (event->key() == Qt::Key_E) {
        state = Zdec;
    } else {
        state = None;
        return;
    }

    cur_key = event->key();
    timer.start(REPAINT_TIME);
}

void Canvas3DViewer::keyReleaseEvent(QKeyEvent *event) {
    if (!timer.isActive() || event->isAutoRepeat())
        return;

    if (event->key() == cur_key){
        timer.stop(), state = None;
    }
}

void Canvas3DViewer::timerAlarm() {
    emit sendModify(state);
}


void Canvas3DViewer::mousePressEvent(QMouseEvent *event) {
    double x = event->pos().x();
    double y = event->pos().y();
    Point p(x,y);
    if (trans) {
        p = trans->fitBack(p);
        Point min = trans->fitBack(Point(0,0));
        Point max = trans->fitBack(Point(width(), height()));
        swap(min.y, max.y);
        double kx = (p.x - min.x) / (max.x - min.x);
        double ky = (p.y - min.y) / (max.y - min.y);

        auto bords = viewer.getBorders();
        p.x = bords.x_min + kx * (bords.x_max - bords.x_min);
        p.y = bords.y_min + ky * (bords.y_max - bords.y_min);
    }

    x = p.x;
    y = p.y;
    emit mousePressed(x, y);
}