//
// Created by kurush on 29.06.2020.
//

#ifndef BIG3DFLUFFY_QRACTIONMANAGER_H
#define BIG3DFLUFFY_QRACTIONMANAGER_H

#include <vector>
#include "../QTIncludes.h"
#include "QRConstants.h"

enum QRKey {
    NOKEY,
    QRKey_W, QRKey_S,
    QRKey_A, QRKey_D,
    QRKey_E, QRKey_R
};
struct QRModifiers {
    bool ctrl, shift, alt, mouseLeftPressed;
};


class QRActionManager: public QGLWidget {
    Q_OBJECT
public:
    QRActionManager(QWidget *parent = nullptr);

signals:
    void QRKeyPressed(QRKey, QRModifiers);
    void QRKeyReleased(QRKey, QRModifiers);

    // todo lkm, pkm, mapping to real coords (not here!)
    void QRMousePressed(float x, float y, QRModifiers);
    void QRMouseMoved(float dx, float dy, QRModifiers);
    void QRMouseWheelMoved(float val, float x, float y, QRModifiers);

private slots:
    void slotBtnTimerAlarm() {isButtonBlocked = false;};
private:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;



    QRModifiers mods;
    QTimer btnTimer;
    bool isButtonBlocked = true;
    float cur_mouse_x = -QRINF, cur_mouse_y = -QRINF;
};


#endif //BIG3DFLUFFY_QRACTIONMANAGER_H
