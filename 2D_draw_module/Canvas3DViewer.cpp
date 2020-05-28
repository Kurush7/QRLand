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
