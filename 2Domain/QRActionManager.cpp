//
// Created by kurush on 29.06.2020.
//

#include "QRActionManager.h"

QRActionManager::QRActionManager(QWidget *parent): QGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    connect(&btnTimer, SIGNAL(timeout()), this, SLOT(slotBtnTimerAlarm()));
    btnTimer.setInterval(BUTTON_REPRESS_TIME_MSEC);
    btnTimer.start();
}

void QRActionManager::keyPressEvent(QKeyEvent *event) {
    if (isButtonBlocked) return;
    isButtonBlocked = true;

    switch(event->key()) {
        case Qt::SHIFT: mods.shift = true; return;
        case Qt::ALT: mods.alt = true; return;
        case Qt::CTRL: mods.ctrl = true; return;
    }
    switch(event->key()) {
        case Qt::Key_W: emit QRKeyPressed(QRKey_W, mods); break;
        case Qt::Key_S: emit QRKeyPressed(QRKey_S, mods); break;
        case Qt::Key_A: emit QRKeyPressed(QRKey_A, mods); break;
        case Qt::Key_D: emit QRKeyPressed(QRKey_D, mods); break;
        case Qt::Key_E: emit QRKeyPressed(QRKey_E, mods); break;
        case Qt::Key_R: emit QRKeyPressed(QRKey_R, mods); break;
    }
}

void QRActionManager::keyReleaseEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::SHIFT: mods.shift = false; return;
        case Qt::ALT: mods.alt = false; return;
        case Qt::CTRL: mods.ctrl = false; return;
    }
    switch(event->key()) {
        case Qt::Key_W: emit QRKeyReleased(QRKey_W, mods); break;
        case Qt::Key_S: emit QRKeyReleased(QRKey_S, mods); break;
        case Qt::Key_A: emit QRKeyReleased(QRKey_A, mods); break;
        case Qt::Key_D: emit QRKeyReleased(QRKey_D, mods); break;
        case Qt::Key_E: emit QRKeyReleased(QRKey_E, mods); break;
        case Qt::Key_R: emit QRKeyReleased(QRKey_R, mods); break;
    }
}


void QRActionManager::mousePressEvent(QMouseEvent *event) {
    float x = event->pos().x();
    float y = event->pos().y();

    emit QRMousePressed(x, y, mods);
}