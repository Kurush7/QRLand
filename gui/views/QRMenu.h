//
// Created by kurush on 17.10.2020.
//

#ifndef BIG3DFLUFFY_QRMENU_H
#define BIG3DFLUFFY_QRMENU_H

#include "QTIncludes.h"
#include <QMenu>
#include <QMenuBar>
#include "QRSettings.h"
#include "../prod/Facade.h"

class QRMenu: public QMenuBar {
    Q_OBJECT

signals:
    void onQuit();
public:
    QRMenu(sptr<Facade> facade=nullptr, QWidget *parent= nullptr);
private:
    sptr<Facade> facade;
    QWidget *open=nullptr;
};


#endif //BIG3DFLUFFY_QRMENU_H
