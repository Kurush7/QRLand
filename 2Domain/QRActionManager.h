//
// Created by kurush on 29.06.2020.
//

#ifndef BIG3DFLUFFY_QRACTIONMANAGER_H
#define BIG3DFLUFFY_QRACTIONMANAGER_H

#include <QGraphicsView>
#include <QWidget>
#include <vector>
#include <QKeyEvent>
#include <QMouseEvent>

enum QRKey {
    NOKEY,
    QRKey_W, QRKey_S,
    QRKey_A, QRKey_D,
    QRKey_E, QRKey_R
};
struct QRModifiers {
    bool ctrl, shift, alt;
};


class QRActionManager: public QWidget {
    Q_OBJECT
public:
    QRActionManager(QWidget *parent = nullptr): QWidget(parent) { setFocusPolicy(Qt::StrongFocus); }

signals:
    void QRKeyPressed(QRKey, QRModifiers);
    void QRKeyReleased(QRKey, QRModifiers);

    // todo lkm, pkm, mapping to real coords (not here!)
    void QRMousePressed(double x, double y, QRModifiers);

private:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;

    QRModifiers mods;
};


#endif //BIG3DFLUFFY_QRACTIONMANAGER_H
